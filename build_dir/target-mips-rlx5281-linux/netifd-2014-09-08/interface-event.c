/*
 * netifd - network interface daemon
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libubox/uloop.h>

#include "netifd.h"
#include "interface.h"
#include "ubus.h"

char *hotplug_cmd_path = DEFAULT_HOTPLUG_PATH;
static struct interface *current;
static enum interface_event current_ev;
static struct list_head pending = LIST_HEAD_INIT(pending);

static void task_complete(struct uloop_process *proc, int ret);
static struct uloop_process task = {
	.cb = task_complete,
};
static const char * const eventnames[] = {"ifdown", "ifup", "ifupdate"};

static void
run_cmd(const char *ifname, const char *device, enum interface_event event,
		enum interface_update_flags updated)
{
	char *argv[3];
	int pid;

	pid = fork();
	if (pid < 0)
		return task_complete(NULL, -1);

	if (pid > 0) {
		task.pid = pid;
		uloop_process_add(&task);
		return;
	}

	setenv("ACTION", eventnames[event], 1);
	setenv("INTERFACE", ifname, 1);
	if (device)
		setenv("DEVICE", device, 1);

	if (event == IFEV_UPDATE) {
		if (updated & IUF_ADDRESS)
			setenv("IFUPDATE_ADDRESSES", "1", 1);
		if (updated & IUF_ROUTE)
			setenv("IFUPDATE_ROUTES", "1", 1);
		if (updated & IUF_PREFIX)
			setenv("IFUPDATE_PREFIXES", "1", 1);
		if (updated & IUF_DATA)
			setenv("IFUPDATE_DATA", "1", 1);
	}

	argv[0] = hotplug_cmd_path;
	argv[1] = "iface";
	argv[2] = NULL;
	execvp(argv[0], argv);
	exit(127);
}

static void
call_hotplug(void)
{
	const char *device = NULL;
	if (list_empty(&pending))
		return;

	current = list_first_entry(&pending, struct interface, hotplug_list);
	current_ev = current->hotplug_ev;
	list_del_init(&current->hotplug_list);

	if ((current_ev == IFEV_UP || current_ev == IFEV_UPDATE) && current->l3_dev.dev)
		device = current->l3_dev.dev->ifname;

	D(SYSTEM, "Call hotplug handler for interface '%s', event '%s' (%s)\n",
	current->name, eventnames[current_ev], device ? device : "none");
	run_cmd(current->name, device, current_ev, current->updated);
}

static void
task_complete(struct uloop_process *proc, int ret)
{
	if (current)
		D(SYSTEM, "Complete hotplug handler for interface '%s'\n", current->name);
	current = NULL;
	call_hotplug();
}

/*
 * Queue an interface for an up/down event.
 * An interface can only have one event in the queue and one
 * event waiting for completion.
 * When queueing an event that is the same as the one waiting for
 * completion, remove the interface from the queue
 */
static void
interface_queue_event(struct interface *iface, enum interface_event ev)
{
	D(SYSTEM, "Queue hotplug handler for interface '%s', event '%s'\n",
			iface->name, eventnames[ev]);
	if (ev == IFEV_UP || ev == IFEV_DOWN)
		netifd_ubus_interface_event(iface, ev == IFEV_UP);

	netifd_ubus_interface_notify(iface, ev != IFEV_DOWN);

	if (current == iface) {
		/* an event for iface is being processed */
		if (!list_empty(&iface->hotplug_list)) {
			/* an additional event for iface is pending */
			if ((ev != current_ev || ev == IFEV_UPDATE) &&
			!(iface->hotplug_ev == IFEV_UP && ev == IFEV_UPDATE)) {
				/* if incoming event is different from the one
				 * being handled or if it is an update,
				 * overwrite pending event, but never
				 * overwrite an ifup with an ifupdate */
				iface->hotplug_ev = ev;
			}
			else if (ev == current_ev && ev != IFEV_UPDATE) {
				/* if incoming event is not an ifupdate
				 * and is the same as the one that is
				 * being handled, remove it from the
				 * pending list */
				list_del_init(&iface->hotplug_list);
			}
		}
		else {
			/* no additional event for iface is pending */
			if (ev != current_ev || ev == IFEV_UPDATE) {
				/* only add the interface to the pending list if
				 * the event is different from the one being
				 * handled or if it is an update */
				iface->hotplug_ev = ev;
				/* Handle hotplug calls FIFO */
				list_add_tail(&iface->hotplug_list, &pending);
			}
		}
	}
	else {
		/* currently not handling an event or handling an event
		 * for another interface */
		if (!list_empty(&iface->hotplug_list)) {
			/* an event for iface is pending */
			if (!(iface->hotplug_ev == IFEV_UP &&
				ev == IFEV_UPDATE)) {
				/* overwrite pending event, unless the incoming
				 * event is an ifupdate while the pending one
				 * is an ifup */
				iface->hotplug_ev = ev;
			}
		}
		else {
			/* an event for the interface is not yet pending,
			 * queue it */
			iface->hotplug_ev = ev;
			/* Handle hotplug calls FIFO */
			list_add_tail(&iface->hotplug_list, &pending);
		}
	}

	if (!task.pending && !current)
		call_hotplug();
}

static void
interface_dequeue_event(struct interface *iface)
{
	if (iface == current)
		current = NULL;

	if (!list_empty(&iface->hotplug_list))
		list_del_init(&iface->hotplug_list);
}

static void interface_event_cb(struct interface_user *dep, struct interface *iface,
			       enum interface_event ev)
{
	switch (ev) {
		case IFEV_UP:
		case IFEV_UPDATE:
		case IFEV_DOWN:
			interface_queue_event(iface, ev);
			break;
		case IFEV_FREE:
		case IFEV_RELOAD:
			interface_dequeue_event(iface);
			break;
	}
}

static struct interface_user event_user = {
	.cb = interface_event_cb
};

static void __init interface_event_init(void)
{
	interface_add_user(&event_user, NULL);
}
