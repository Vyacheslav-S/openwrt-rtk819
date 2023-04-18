/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.
 *
 *   Provided by fon.com
 *   Copyright (C) 2008 John Crispin <blogic@openwrt.org> 
 */

#include <string.h>
#include <stdlib.h>

#include "include/log.h"
#include <uci.h>

struct uci_package *p = NULL;

struct uci_context* uci_init(char *config_file)
{
	struct uci_context *ctx = uci_alloc_context();
	uci_add_delta_path(ctx, "/var/state");
	if(uci_load(ctx, config_file, &p) != UCI_OK)
	{
		log_printf("/etc/config/%s is missing or corrupt\n", config_file);
		exit(-1);
	}
	return ctx;
}

void uci_cleanup(struct uci_context *ctx)
{
	uci_unload(ctx, p);
	uci_free_context(ctx);
}

void uci_save_state(struct uci_context *ctx)
{
	uci_save(ctx, p);
}

char* uci_get_option(struct uci_context *ctx, char *section, char *option)
{
	struct uci_element *e = NULL;
	char *value = NULL;
	struct uci_ptr ptr;

	memset(&ptr, 0, sizeof(ptr));
	ptr.package = p->e.name;
	ptr.section = section;
	ptr.option = option;
	if (uci_lookup_ptr(ctx, &ptr, NULL, true) != UCI_OK)
		return NULL;

	if (!(ptr.flags & UCI_LOOKUP_COMPLETE))
		return NULL;

	e = ptr.last;
	switch (e->type)
	{
	case UCI_TYPE_SECTION:
		value = uci_to_section(e)->type;
		break;
	case UCI_TYPE_OPTION:
		switch(ptr.o->type) {
			case UCI_TYPE_STRING:
				value = ptr.o->v.string;
				break;
			default:
				value = NULL;
				break;
		}
		break;
	default:
		return 0;
	}

	return value;
}

int uci_get_option_int(struct uci_context *ctx, char *section, char *option, int def)
{
	char *tmp = uci_get_option(ctx, section, option);
	int ret = def;

	if (tmp)
		ret = atoi(tmp);
	return ret;
}

void uci_for_each_section_type(char *type, void (*cb)(char*, void*), void *priv)
{
	struct uci_element *e;

	uci_foreach_element(&p->sections, e)
		if (!strcmp(type, uci_to_section(e)->type))
			cb(e->name, priv);
}
