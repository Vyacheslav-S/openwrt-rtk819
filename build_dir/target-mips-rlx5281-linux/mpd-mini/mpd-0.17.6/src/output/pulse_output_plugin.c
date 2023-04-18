/*
 * Copyright (C) 2003-2011 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "config.h"
#include "pulse_output_plugin.h"
#include "output_api.h"
#include "mixer_list.h"
#include "mixer/pulse_mixer_plugin.h"

#include <glib.h>

#include <pulse/thread-mainloop.h>
#include <pulse/context.h>
#include <pulse/stream.h>
#include <pulse/introspect.h>
#include <pulse/subscribe.h>
#include <pulse/error.h>
#include <pulse/version.h>

#include <assert.h>
#include <stddef.h>

#define MPD_PULSE_NAME "Music Player Daemon"

#if !defined(PA_CHECK_VERSION)
/**
 * This macro was implemented in libpulse 0.9.16.
 */
#define PA_CHECK_VERSION(a,b,c) false
#endif

struct pulse_output {
	struct audio_output base;

	const char *name;
	const char *server;
	const char *sink;

	struct pulse_mixer *mixer;

	struct pa_threaded_mainloop *mainloop;
	struct pa_context *context;
	struct pa_stream *stream;

	size_t writable;

#if !PA_CHECK_VERSION(0,9,11)
	/**
	 * We need this variable because pa_stream_is_corked() wasn't
	 * added before 0.9.11.
	 */
	bool pause;
#endif
};

/**
 * The quark used for GError.domain.
 */
static inline GQuark
pulse_output_quark(void)
{
	return g_quark_from_static_string("pulse_output");
}

void
pulse_output_lock(struct pulse_output *po)
{
	pa_threaded_mainloop_lock(po->mainloop);
}

void
pulse_output_unlock(struct pulse_output *po)
{
	pa_threaded_mainloop_unlock(po->mainloop);
}

void
pulse_output_set_mixer(struct pulse_output *po, struct pulse_mixer *pm)
{
	assert(po != NULL);
	assert(po->mixer == NULL);
	assert(pm != NULL);

	po->mixer = pm;

	if (po->mainloop == NULL)
		return;

	pa_threaded_mainloop_lock(po->mainloop);

	if (po->context != NULL &&
	    pa_context_get_state(po->context) == PA_CONTEXT_READY) {
		pulse_mixer_on_connect(pm, po->context);

		if (po->stream != NULL &&
		    pa_stream_get_state(po->stream) == PA_STREAM_READY)
			pulse_mixer_on_change(pm, po->context, po->stream);
	}

	pa_threaded_mainloop_unlock(po->mainloop);
}

void
pulse_output_clear_mixer(struct pulse_output *po,
			 G_GNUC_UNUSED struct pulse_mixer *pm)
{
	assert(po != NULL);
	assert(pm != NULL);
	assert(po->mixer == pm);

	po->mixer = NULL;
}

bool
pulse_output_set_volume(struct pulse_output *po,
			const struct pa_cvolume *volume, GError **error_r)
{
	pa_operation *o;

	if (po->context == NULL || po->stream == NULL ||
	    pa_stream_get_state(po->stream) != PA_STREAM_READY) {
		g_set_error(error_r, pulse_output_quark(), 0, "disconnected");
		return false;
	}

	o = pa_context_set_sink_input_volume(po->context,
					     pa_stream_get_index(po->stream),
					     volume, NULL, NULL);
	if (o == NULL) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "failed to set PulseAudio volume: %s",
			    pa_strerror(pa_context_errno(po->context)));
		return false;
	}

	pa_operation_unref(o);
	return true;
}

/**
 * \brief waits for a pulseaudio operation to finish, frees it and
 *     unlocks the mainloop
 * \param operation the operation to wait for
 * \return true if operation has finished normally (DONE state),
 *     false otherwise
 */
static bool
pulse_wait_for_operation(struct pa_threaded_mainloop *mainloop,
			 struct pa_operation *operation)
{
	pa_operation_state_t state;

	assert(mainloop != NULL);
	assert(operation != NULL);

	state = pa_operation_get_state(operation);
	while (state == PA_OPERATION_RUNNING) {
		pa_threaded_mainloop_wait(mainloop);
		state = pa_operation_get_state(operation);
	}

	pa_operation_unref(operation);

	return state == PA_OPERATION_DONE;
}

/**
 * Callback function for stream operation.  It just sends a signal to
 * the caller thread, to wake pulse_wait_for_operation() up.
 */
static void
pulse_output_stream_success_cb(G_GNUC_UNUSED pa_stream *s,
			       G_GNUC_UNUSED int success, void *userdata)
{
	struct pulse_output *po = userdata;

	pa_threaded_mainloop_signal(po->mainloop, 0);
}

static void
pulse_output_context_state_cb(struct pa_context *context, void *userdata)
{
	struct pulse_output *po = userdata;

	switch (pa_context_get_state(context)) {
	case PA_CONTEXT_READY:
		if (po->mixer != NULL)
			pulse_mixer_on_connect(po->mixer, context);

		pa_threaded_mainloop_signal(po->mainloop, 0);
		break;

	case PA_CONTEXT_TERMINATED:
	case PA_CONTEXT_FAILED:
		if (po->mixer != NULL)
			pulse_mixer_on_disconnect(po->mixer);

		/* the caller thread might be waiting for these
		   states */
		pa_threaded_mainloop_signal(po->mainloop, 0);
		break;

	case PA_CONTEXT_UNCONNECTED:
	case PA_CONTEXT_CONNECTING:
	case PA_CONTEXT_AUTHORIZING:
	case PA_CONTEXT_SETTING_NAME:
		break;
	}
}

static void
pulse_output_subscribe_cb(pa_context *context,
			  pa_subscription_event_type_t t,
			  uint32_t idx, void *userdata)
{
	struct pulse_output *po = userdata;
	pa_subscription_event_type_t facility
		= t & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;
	pa_subscription_event_type_t type
		= t & PA_SUBSCRIPTION_EVENT_TYPE_MASK;

	if (po->mixer != NULL &&
	    facility == PA_SUBSCRIPTION_EVENT_SINK_INPUT &&
	    po->stream != NULL &&
	    pa_stream_get_state(po->stream) == PA_STREAM_READY &&
	    idx == pa_stream_get_index(po->stream) &&
	    (type == PA_SUBSCRIPTION_EVENT_NEW ||
	     type == PA_SUBSCRIPTION_EVENT_CHANGE))
		pulse_mixer_on_change(po->mixer, context, po->stream);
}

/**
 * Attempt to connect asynchronously to the PulseAudio server.
 *
 * @return true on success, false on error
 */
static bool
pulse_output_connect(struct pulse_output *po, GError **error_r)
{
	assert(po != NULL);
	assert(po->context != NULL);

	int error;

	error = pa_context_connect(po->context, po->server,
				   (pa_context_flags_t)0, NULL);
	if (error < 0) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_context_connect() has failed: %s",
			    pa_strerror(pa_context_errno(po->context)));
		return false;
	}

	return true;
}

/**
 * Frees and clears the stream.
 */
static void
pulse_output_delete_stream(struct pulse_output *po)
{
	assert(po != NULL);
	assert(po->stream != NULL);

#if PA_CHECK_VERSION(0,9,8)
	pa_stream_set_suspended_callback(po->stream, NULL, NULL);
#endif

	pa_stream_set_state_callback(po->stream, NULL, NULL);
	pa_stream_set_write_callback(po->stream, NULL, NULL);

	pa_stream_disconnect(po->stream);
	pa_stream_unref(po->stream);
	po->stream = NULL;
}

/**
 * Frees and clears the context.
 *
 * Caller must lock the main loop.
 */
static void
pulse_output_delete_context(struct pulse_output *po)
{
	assert(po != NULL);
	assert(po->context != NULL);

	pa_context_set_state_callback(po->context, NULL, NULL);
	pa_context_set_subscribe_callback(po->context, NULL, NULL);

	pa_context_disconnect(po->context);
	pa_context_unref(po->context);
	po->context = NULL;
}

/**
 * Create, set up and connect a context.
 *
 * Caller must lock the main loop.
 *
 * @return true on success, false on error
 */
static bool
pulse_output_setup_context(struct pulse_output *po, GError **error_r)
{
	assert(po != NULL);
	assert(po->mainloop != NULL);

	po->context = pa_context_new(pa_threaded_mainloop_get_api(po->mainloop),
				     MPD_PULSE_NAME);
	if (po->context == NULL) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_context_new() has failed");
		return false;
	}

	pa_context_set_state_callback(po->context,
				      pulse_output_context_state_cb, po);
	pa_context_set_subscribe_callback(po->context,
					  pulse_output_subscribe_cb, po);

	if (!pulse_output_connect(po, error_r)) {
		pulse_output_delete_context(po);
		return false;
	}

	return true;
}

static struct audio_output *
pulse_output_init(const struct config_param *param, GError **error_r)
{
	struct pulse_output *po;

	g_setenv("PULSE_PROP_media.role", "music", true);

	po = g_new(struct pulse_output, 1);
	if (!ao_base_init(&po->base, &pulse_output_plugin, param, error_r)) {
		g_free(po);
		return NULL;
	}

	po->name = config_get_block_string(param, "name", "mpd_pulse");
	po->server = config_get_block_string(param, "server", NULL);
	po->sink = config_get_block_string(param, "sink", NULL);

	po->mixer = NULL;
	po->mainloop = NULL;
	po->context = NULL;
	po->stream = NULL;

	return &po->base;
}

static void
pulse_output_finish(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;

	ao_base_finish(&po->base);
	g_free(po);
}

static bool
pulse_output_enable(struct audio_output *ao, GError **error_r)
{
	struct pulse_output *po = (struct pulse_output *)ao;

	assert(po->mainloop == NULL);
	assert(po->context == NULL);

	/* create the libpulse mainloop and start the thread */

	po->mainloop = pa_threaded_mainloop_new();
	if (po->mainloop == NULL) {
		g_free(po);

		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_threaded_mainloop_new() has failed");
		return false;
	}

	pa_threaded_mainloop_lock(po->mainloop);

	if (pa_threaded_mainloop_start(po->mainloop) < 0) {
		pa_threaded_mainloop_unlock(po->mainloop);
		pa_threaded_mainloop_free(po->mainloop);
		po->mainloop = NULL;

		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_threaded_mainloop_start() has failed");
		return false;
	}

	/* create the libpulse context and connect it */

	if (!pulse_output_setup_context(po, error_r)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		pa_threaded_mainloop_stop(po->mainloop);
		pa_threaded_mainloop_free(po->mainloop);
		po->mainloop = NULL;
		return false;
	}

	pa_threaded_mainloop_unlock(po->mainloop);

	return true;
}

static void
pulse_output_disable(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;

	assert(po->mainloop != NULL);

	pa_threaded_mainloop_stop(po->mainloop);
	if (po->context != NULL)
		pulse_output_delete_context(po);
	pa_threaded_mainloop_free(po->mainloop);
	po->mainloop = NULL;
}

/**
 * Check if the context is (already) connected, and waits if not.  If
 * the context has been disconnected, retry to connect.
 *
 * Caller must lock the main loop.
 *
 * @return true on success, false on error
 */
static bool
pulse_output_wait_connection(struct pulse_output *po, GError **error_r)
{
	assert(po->mainloop != NULL);

	pa_context_state_t state;

	if (po->context == NULL && !pulse_output_setup_context(po, error_r))
		return false;

	while (true) {
		state = pa_context_get_state(po->context);
		switch (state) {
		case PA_CONTEXT_READY:
			/* nothing to do */
			return true;

		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_TERMINATED:
		case PA_CONTEXT_FAILED:
			/* failure */
			g_set_error(error_r, pulse_output_quark(), 0,
				    "failed to connect: %s",
				    pa_strerror(pa_context_errno(po->context)));
			pulse_output_delete_context(po);
			return false;

		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			/* wait some more */
			pa_threaded_mainloop_wait(po->mainloop);
			break;
		}
	}
}

#if PA_CHECK_VERSION(0,9,8)

static void
pulse_output_stream_suspended_cb(G_GNUC_UNUSED pa_stream *stream, void *userdata)
{
	struct pulse_output *po = userdata;

	assert(stream == po->stream || po->stream == NULL);
	assert(po->mainloop != NULL);

	/* wake up the main loop to break out of the loop in
	   pulse_output_play() */
	pa_threaded_mainloop_signal(po->mainloop, 0);
}

#endif

static void
pulse_output_stream_state_cb(pa_stream *stream, void *userdata)
{
	struct pulse_output *po = userdata;

	assert(stream == po->stream || po->stream == NULL);
	assert(po->mainloop != NULL);
	assert(po->context != NULL);

	switch (pa_stream_get_state(stream)) {
	case PA_STREAM_READY:
		if (po->mixer != NULL)
			pulse_mixer_on_change(po->mixer, po->context, stream);

		pa_threaded_mainloop_signal(po->mainloop, 0);
		break;

	case PA_STREAM_FAILED:
	case PA_STREAM_TERMINATED:
		if (po->mixer != NULL)
			pulse_mixer_on_disconnect(po->mixer);

		pa_threaded_mainloop_signal(po->mainloop, 0);
		break;

	case PA_STREAM_UNCONNECTED:
	case PA_STREAM_CREATING:
		break;
	}
}

static void
pulse_output_stream_write_cb(G_GNUC_UNUSED pa_stream *stream, size_t nbytes,
			     void *userdata)
{
	struct pulse_output *po = userdata;

	assert(po->mainloop != NULL);

	po->writable = nbytes;
	pa_threaded_mainloop_signal(po->mainloop, 0);
}

/**
 * Create, set up and connect a context.
 *
 * Caller must lock the main loop.
 *
 * @return true on success, false on error
 */
static bool
pulse_output_setup_stream(struct pulse_output *po, const pa_sample_spec *ss,
			  GError **error_r)
{
	assert(po != NULL);
	assert(po->context != NULL);

	po->stream = pa_stream_new(po->context, po->name, ss, NULL);
	if (po->stream == NULL) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_stream_new() has failed: %s",
			    pa_strerror(pa_context_errno(po->context)));
		return false;
	}

#if PA_CHECK_VERSION(0,9,8)
	pa_stream_set_suspended_callback(po->stream,
					 pulse_output_stream_suspended_cb, po);
#endif

	pa_stream_set_state_callback(po->stream,
				     pulse_output_stream_state_cb, po);
	pa_stream_set_write_callback(po->stream,
				     pulse_output_stream_write_cb, po);

	return true;
}

static bool
pulse_output_open(struct audio_output *ao, struct audio_format *audio_format,
		  GError **error_r)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	pa_sample_spec ss;
	int error;

	assert(po->mainloop != NULL);

	pa_threaded_mainloop_lock(po->mainloop);

	if (po->context != NULL) {
		switch (pa_context_get_state(po->context)) {
		case PA_CONTEXT_UNCONNECTED:
		case PA_CONTEXT_TERMINATED:
		case PA_CONTEXT_FAILED:
			/* the connection was closed meanwhile; delete
			   it, and pulse_output_wait_connection() will
			   reopen it */
			pulse_output_delete_context(po);
			break;

		case PA_CONTEXT_READY:
		case PA_CONTEXT_CONNECTING:
		case PA_CONTEXT_AUTHORIZING:
		case PA_CONTEXT_SETTING_NAME:
			break;
		}
	}

	if (!pulse_output_wait_connection(po, error_r)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		return false;
	}

	/* MPD doesn't support the other pulseaudio sample formats, so
	   we just force MPD to send us everything as 16 bit */
	audio_format->format = SAMPLE_FORMAT_S16;

	ss.format = PA_SAMPLE_S16NE;
	ss.rate = audio_format->sample_rate;
	ss.channels = audio_format->channels;

	/* create a stream .. */

	if (!pulse_output_setup_stream(po, &ss, error_r)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		return false;
	}

	/* .. and connect it (asynchronously) */

	error = pa_stream_connect_playback(po->stream, po->sink,
					   NULL, 0, NULL, NULL);
	if (error < 0) {
		pulse_output_delete_stream(po);

		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_stream_connect_playback() has failed: %s",
			    pa_strerror(pa_context_errno(po->context)));
		pa_threaded_mainloop_unlock(po->mainloop);
		return false;
	}

	pa_threaded_mainloop_unlock(po->mainloop);

#if !PA_CHECK_VERSION(0,9,11)
	po->pause = false;
#endif

	return true;
}

static void
pulse_output_close(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	pa_operation *o;

	assert(po->mainloop != NULL);

	pa_threaded_mainloop_lock(po->mainloop);

	if (pa_stream_get_state(po->stream) == PA_STREAM_READY) {
		o = pa_stream_drain(po->stream,
				    pulse_output_stream_success_cb, po);
		if (o == NULL) {
			g_warning("pa_stream_drain() has failed: %s",
				  pa_strerror(pa_context_errno(po->context)));
		} else
			pulse_wait_for_operation(po->mainloop, o);
	}

	pulse_output_delete_stream(po);

	if (po->context != NULL &&
	    pa_context_get_state(po->context) != PA_CONTEXT_READY)
		pulse_output_delete_context(po);

	pa_threaded_mainloop_unlock(po->mainloop);
}

/**
 * Check if the stream is (already) connected, and waits if not.  The
 * mainloop must be locked before calling this function.
 *
 * @return true on success, false on error
 */
static bool
pulse_output_wait_stream(struct pulse_output *po, GError **error_r)
{
	while (true) {
		switch (pa_stream_get_state(po->stream)) {
		case PA_STREAM_READY:
			return true;

		case PA_STREAM_FAILED:
		case PA_STREAM_TERMINATED:
		case PA_STREAM_UNCONNECTED:
			g_set_error(error_r, pulse_output_quark(),
				    pa_context_errno(po->context),
				    "failed to connect the stream: %s",
				    pa_strerror(pa_context_errno(po->context)));
			return false;

		case PA_STREAM_CREATING:
			pa_threaded_mainloop_wait(po->mainloop);
			break;
		}
	}
}

/**
 * Determines whether the stream is paused.  On libpulse older than
 * 0.9.11, it uses a custom pause flag.
 */
static bool
pulse_output_stream_is_paused(struct pulse_output *po)
{
	assert(po->stream != NULL);

#if !defined(PA_CHECK_VERSION) || !PA_CHECK_VERSION(0,9,11)
	return po->pause;
#else
	return pa_stream_is_corked(po->stream);
#endif
}

/**
 * Sets cork mode on the stream.
 */
static bool
pulse_output_stream_pause(struct pulse_output *po, bool pause,
			  GError **error_r)
{
	pa_operation *o;

	assert(po->mainloop != NULL);
	assert(po->context != NULL);
	assert(po->stream != NULL);

	o = pa_stream_cork(po->stream, pause,
			   pulse_output_stream_success_cb, po);
	if (o == NULL) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_stream_cork() has failed: %s",
			    pa_strerror(pa_context_errno(po->context)));
		return false;
	}

	if (!pulse_wait_for_operation(po->mainloop, o)) {
		g_set_error(error_r, pulse_output_quark(), 0,
			    "pa_stream_cork() has failed: %s",
			    pa_strerror(pa_context_errno(po->context)));
		return false;
	}

#if !PA_CHECK_VERSION(0,9,11)
	po->pause = pause;
#endif
	return true;
}

static unsigned
pulse_output_delay(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	unsigned result = 0;

	pa_threaded_mainloop_lock(po->mainloop);

	if (po->base.pause && pulse_output_stream_is_paused(po) &&
	    pa_stream_get_state(po->stream) == PA_STREAM_READY)
		/* idle while paused */
		result = 1000;

	pa_threaded_mainloop_unlock(po->mainloop);

	return result;
}

static size_t
pulse_output_play(struct audio_output *ao, const void *chunk, size_t size,
		  GError **error_r)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	int error;

	assert(po->mainloop != NULL);
	assert(po->stream != NULL);

	pa_threaded_mainloop_lock(po->mainloop);

	/* check if the stream is (already) connected */

	if (!pulse_output_wait_stream(po, error_r)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		return 0;
	}

	assert(po->context != NULL);

	/* unpause if previously paused */

	if (pulse_output_stream_is_paused(po) &&
	    !pulse_output_stream_pause(po, false, error_r)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		return 0;
	}

	/* wait until the server allows us to write */

	while (po->writable == 0) {
#if PA_CHECK_VERSION(0,9,8)
		if (pa_stream_is_suspended(po->stream)) {
			pa_threaded_mainloop_unlock(po->mainloop);
			g_set_error(error_r, pulse_output_quark(), 0,
				    "suspended");
			return 0;
		}
#endif

		pa_threaded_mainloop_wait(po->mainloop);

		if (pa_stream_get_state(po->stream) != PA_STREAM_READY) {
			pa_threaded_mainloop_unlock(po->mainloop);
			g_set_error(error_r, pulse_output_quark(), 0,
				    "disconnected");
			return 0;
		}
	}

	/* now write */

	if (size > po->writable)
		/* don't send more than possible */
		size = po->writable;

	po->writable -= size;

	error = pa_stream_write(po->stream, chunk, size, NULL,
				0, PA_SEEK_RELATIVE);
	pa_threaded_mainloop_unlock(po->mainloop);
	if (error < 0) {
		g_set_error(error_r, pulse_output_quark(), error,
			    "%s", pa_strerror(error));
		return 0;
	}

	return size;
}

static void
pulse_output_cancel(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	pa_operation *o;

	assert(po->mainloop != NULL);
	assert(po->stream != NULL);

	pa_threaded_mainloop_lock(po->mainloop);

	if (pa_stream_get_state(po->stream) != PA_STREAM_READY) {
		/* no need to flush when the stream isn't connected
		   yet */
		pa_threaded_mainloop_unlock(po->mainloop);
		return;
	}

	assert(po->context != NULL);

	o = pa_stream_flush(po->stream, pulse_output_stream_success_cb, po);
	if (o == NULL) {
		g_warning("pa_stream_flush() has failed: %s",
			  pa_strerror(pa_context_errno(po->context)));
		pa_threaded_mainloop_unlock(po->mainloop);
		return;
	}

	pulse_wait_for_operation(po->mainloop, o);
	pa_threaded_mainloop_unlock(po->mainloop);
}

static bool
pulse_output_pause(struct audio_output *ao)
{
	struct pulse_output *po = (struct pulse_output *)ao;
	GError *error = NULL;

	assert(po->mainloop != NULL);
	assert(po->stream != NULL);

	pa_threaded_mainloop_lock(po->mainloop);

	/* check if the stream is (already/still) connected */

	if (!pulse_output_wait_stream(po, &error)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		g_warning("%s", error->message);
		g_error_free(error);
		return false;
	}

	assert(po->context != NULL);

	/* cork the stream */

	if (!pulse_output_stream_is_paused(po) &&
	    !pulse_output_stream_pause(po, true, &error)) {
		pa_threaded_mainloop_unlock(po->mainloop);
		g_warning("%s", error->message);
		g_error_free(error);
		return false;
	}

	pa_threaded_mainloop_unlock(po->mainloop);

	return true;
}

static bool
pulse_output_test_default_device(void)
{
	struct pulse_output *po;
	bool success;

	po = (struct pulse_output *)pulse_output_init(NULL, NULL);
	if (po == NULL)
		return false;

	success = pulse_output_wait_connection(po, NULL);
	pulse_output_finish(&po->base);

	return success;
}

const struct audio_output_plugin pulse_output_plugin = {
	.name = "pulse",

	.test_default_device = pulse_output_test_default_device,
	.init = pulse_output_init,
	.finish = pulse_output_finish,
	.enable = pulse_output_enable,
	.disable = pulse_output_disable,
	.open = pulse_output_open,
	.delay = pulse_output_delay,
	.play = pulse_output_play,
	.cancel = pulse_output_cancel,
	.pause = pulse_output_pause,
	.close = pulse_output_close,

	.mixer_plugin = &pulse_mixer_plugin,
};
