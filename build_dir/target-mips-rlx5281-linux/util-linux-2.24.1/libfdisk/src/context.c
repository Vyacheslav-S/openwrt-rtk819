
#include "fdiskP.h"

struct fdisk_context *fdisk_new_context(void)
{
	struct fdisk_context *cxt;

	cxt = calloc(1, sizeof(*cxt));
	if (!cxt)
		return NULL;

	DBG(LABEL, dbgprint("new context %p allocated", cxt));
	cxt->dev_fd = -1;

	/*
	 * Allocate label specific structs.
	 *
	 * This is necessary (for example) to store label specific
	 * context setting.
	 */
	cxt->labels[ cxt->nlabels++ ] = fdisk_new_gpt_label(cxt);
	cxt->labels[ cxt->nlabels++ ] = fdisk_new_dos_label(cxt);
	cxt->labels[ cxt->nlabels++ ] = fdisk_new_bsd_label(cxt);
	cxt->labels[ cxt->nlabels++ ] = fdisk_new_sgi_label(cxt);
	cxt->labels[ cxt->nlabels++ ] = fdisk_new_sun_label(cxt);

	return cxt;
}

/* only BSD is supported now */
struct fdisk_context *fdisk_new_nested_context(struct fdisk_context *parent,
				const char *name)
{
	struct fdisk_context *cxt;
	struct fdisk_label *lb = NULL;

	assert(parent);

	cxt = calloc(1, sizeof(*cxt));
	if (!cxt)
		return NULL;

	DBG(LABEL, dbgprint("new context %p allocated", cxt));
	cxt->dev_fd = parent->dev_fd;
	cxt->parent = parent;

	cxt->io_size =          parent->io_size;
	cxt->optimal_io_size =  parent->optimal_io_size;
	cxt->min_io_size =      parent->min_io_size;
	cxt->phy_sector_size =  parent->phy_sector_size;
	cxt->sector_size =      parent->sector_size;
	cxt->alignment_offset = parent->alignment_offset;
	cxt->grain =            parent->grain;
	cxt->first_lba =        parent->first_lba;
	cxt->total_sectors =    parent->total_sectors;

	cxt->ask_cb =		parent->ask_cb;
	cxt->ask_data =		parent->ask_data;

	cxt->geom = parent->geom;

	if (name && strcmp(name, "bsd") == 0)
		lb = cxt->labels[ cxt->nlabels++ ] = fdisk_new_bsd_label(cxt);

	if (lb) {
		DBG(LABEL, dbgprint("probing for nested %s", lb->name));

		cxt->label = lb;

		if (lb->op->probe(cxt) == 1)
			__fdisk_context_switch_label(cxt, lb);
		else {
			DBG(LABEL, dbgprint("not found %s label", lb->name));
			if (lb->op->deinit)
				lb->op->deinit(lb);
			cxt->label = NULL;
		}
	}

	return cxt;
}


/*
 * Returns the current label if no name specified.
 */
struct fdisk_label *fdisk_context_get_label(struct fdisk_context *cxt, const char *name)
{
	size_t i;

	assert(cxt);

	if (!name)
		return cxt->label;

	for (i = 0; i < cxt->nlabels; i++)
		if (cxt->labels[i]
		    && strcmp(cxt->labels[i]->name, name) == 0)
			return cxt->labels[i];

	DBG(LABEL, dbgprint("failed to found %s label driver\n", name));
	return NULL;
}

int fdisk_context_next_label(struct fdisk_context *cxt, struct fdisk_label **lb)
{
	size_t i;
	struct fdisk_label *res = NULL;

	if (!lb || !cxt)
		return -EINVAL;

	if (!*lb)
		res = cxt->labels[0];
	else {
		for (i = 1; i < cxt->nlabels; i++) {
			if (*lb == cxt->labels[i - 1]) {
				res = cxt->labels[i];
				break;
			}
		}
	}

	*lb = res;
	return res ? 0 : 1;
}

int __fdisk_context_switch_label(struct fdisk_context *cxt,
				 struct fdisk_label *lb)
{
	if (!lb || !cxt)
		return -EINVAL;
	if (lb->disabled) {
		DBG(LABEL, dbgprint("*** attempt to switch to disabled label %s -- ignore!", lb->name));
		return -EINVAL;
	}
	cxt->label = lb;
	DBG(LABEL, dbgprint("--> switching context to %s!", lb->name));
	return 0;
}

int fdisk_context_switch_label(struct fdisk_context *cxt, const char *name)
{
	return __fdisk_context_switch_label(cxt,
			fdisk_context_get_label(cxt, name));
}


static void reset_context(struct fdisk_context *cxt)
{
	size_t i;

	DBG(CONTEXT, dbgprint("*** resetting context %p", cxt));

	/* reset drives' private data */
	for (i = 0; i < cxt->nlabels; i++)
		fdisk_deinit_label(cxt->labels[i]);

	/* free device specific stuff */
	if (!cxt->parent && cxt->dev_fd > -1)
		close(cxt->dev_fd);
	free(cxt->dev_path);
	free(cxt->firstsector);

	/* initialize */
	cxt->dev_fd = -1;
	cxt->dev_path = NULL;
	cxt->firstsector = NULL;

	fdisk_zeroize_device_properties(cxt);

	cxt->label = NULL;
}

/**
 * fdisk_context_assign_device:
 * @fname: path to the device to be handled
 * @readonly: how to open the device
 *
 * If the @readonly flag is set to false, fdisk will attempt to open
 * the device with read-write mode and will fallback to read-only if
 * unsuccessful.
 *
 * Returns: 0 on success, < 0 on error.
 */
int fdisk_context_assign_device(struct fdisk_context *cxt,
				const char *fname, int readonly)
{
	int fd;

	DBG(CONTEXT, dbgprint("assigning device %s", fname));
	assert(cxt);

	reset_context(cxt);

	if (readonly == 1 || (fd = open(fname, O_RDWR|O_CLOEXEC)) < 0) {
		if ((fd = open(fname, O_RDONLY|O_CLOEXEC)) < 0)
			return -errno;
		readonly = 1;
	}

	cxt->dev_fd = fd;
	cxt->dev_path = strdup(fname);
	if (!cxt->dev_path)
		goto fail;

	fdisk_discover_topology(cxt);
	fdisk_discover_geometry(cxt);

	if (fdisk_read_firstsector(cxt) < 0)
		goto fail;

	/* detect labels and apply labes specific stuff (e.g geomery)
	 * to the context */
	fdisk_probe_labels(cxt);

	/* let's apply user geometry *after* label prober
	 * to make it possible to override in-label setting */
	fdisk_apply_user_device_properties(cxt);

	DBG(CONTEXT, dbgprint("context %p initialized for %s [%s]",
			      cxt, fname,
			      readonly ? "READ-ONLY" : "READ-WRITE"));
	return 0;
fail:
	DBG(CONTEXT, dbgprint("failed to assign device"));
	return -errno;
}

int fdisk_context_deassign_device(struct fdisk_context *cxt)
{
	assert(cxt);
	assert(cxt->dev_fd >= 0);

	if (fsync(cxt->dev_fd) || close(cxt->dev_fd)) {
		fdisk_warn(cxt, _("%s: close device failed"), cxt->dev_path);
		return -errno;
	}

	fdisk_info(cxt, _("Syncing disks."));
	sync();

	cxt->dev_fd = -1;
	return 0;
}

/**
 * fdisk_free_context:
 * @cxt: fdisk context
 *
 * Deallocates context struct.
 */
void fdisk_free_context(struct fdisk_context *cxt)
{
	int i;

	if (!cxt)
		return;

	DBG(CONTEXT, dbgprint("freeing context %p for %s", cxt, cxt->dev_path));
	reset_context(cxt);

	/* deallocate label's private stuff */
	for (i = 0; i < cxt->nlabels; i++) {
		if (!cxt->labels[i])
			continue;
		if (cxt->labels[i]->op->free)
			cxt->labels[i]->op->free(cxt->labels[i]);
		else
			free(cxt->labels[i]);
	}

	free(cxt);
}

/**
 * fdisk_context_set_ask:
 * @cxt: context
 * @ask_cb: callback
 * @data: callback data
 *
 * Returns: 0 on success, < 0 on error.
 */
int fdisk_context_set_ask(struct fdisk_context *cxt,
		int (*ask_cb)(struct fdisk_context *, struct fdisk_ask *, void *),
		void *data)
{
	assert(cxt);

	cxt->ask_cb = ask_cb;
	cxt->ask_data = data;
	return 0;
}

/**
 * fdisk_context_enable_details:
 * cxt: context
 * enable: true/flase
 *
 * Enables or disables "details" display mode.
 *
 * Returns: 0 on success, < 0 on error.
 */
int fdisk_context_enable_details(struct fdisk_context *cxt, int enable)
{
	assert(cxt);
	cxt->display_details = enable ? 1 : 0;
	return 0;
}

int fdisk_context_display_details(struct fdisk_context *cxt)
{
	assert(cxt);
	return cxt->display_details == 1;
}

/**
 * fdisk_context_enable_listonly:
 * cxt: context
 * enable: true/flase
 *
 * Just list partition only, don't care about another details, mistakes, ...
 *
 * Returns: 0 on success, < 0 on error.
 */
int fdisk_context_enable_listonly(struct fdisk_context *cxt, int enable)
{
	assert(cxt);
	cxt->listonly = enable ? 1 : 0;
	return 0;
}

int fdisk_context_listonly(struct fdisk_context *cxt)
{
	assert(cxt);
	return cxt->listonly == 1;
}


/*
 * @str: "cylinder" or "sector".
 *
 * This is pure shit, unfortunately for example Sun addresses begin of the
 * partition by cylinders...
 */
int fdisk_context_set_unit(struct fdisk_context *cxt, const char *str)
{
	assert(cxt);

	cxt->display_in_cyl_units = 0;

	if (!str)
		return 0;

	if (strcmp(str, "cylinder") == 0 || strcmp(str, "cylinders") == 0)
		cxt->display_in_cyl_units = 1;

	else if (strcmp(str, "sector") == 0 || strcmp(str, "sectors") == 0)
		cxt->display_in_cyl_units = 0;

	DBG(CONTEXT, dbgprint("display unit: %s", fdisk_context_get_unit(cxt, 0)));
	return 0;
}

const char *fdisk_context_get_unit(struct fdisk_context *cxt, int n)
{
	assert(cxt);

	if (fdisk_context_use_cylinders(cxt))
		return P_("cylinder", "cylinders", n);
	return P_("sector", "sectors", n);
}

/* Returns 1 if user wants to display in cylinders. */
int fdisk_context_use_cylinders(struct fdisk_context *cxt)
{
	assert(cxt);
	return cxt->display_in_cyl_units == 1;
}

/* Returns number of "units" per sector, default is 1 if display unit is sector.
 */
unsigned int fdisk_context_get_units_per_sector(struct fdisk_context *cxt)
{
	assert(cxt);

	if (fdisk_context_use_cylinders(cxt)) {
		assert(cxt->geom.heads);
		return cxt->geom.heads * cxt->geom.sectors;
	}
	return 1;
}
