/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
 * Copyright (C) 2012 Red Hat
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef __GIS_PAGE_MANAGER_H__
#define __GIS_PAGE_MANAGER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GIS_TYPE_PAGE_MANAGER         (gis_page_manager_get_type ())
#define GIS_PAGE_MANAGER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GIS_TYPE_PAGE_MANAGER, GisPageManager))
#define GIS_PAGE_MANAGER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GIS_TYPE_PAGE_MANAGER, GisPageManagerClass))
#define GIS_IS_PAGE_MANAGER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GIS_TYPE_PAGE_MANAGER))
#define GIS_IS_PAGE_MANAGER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GIS_TYPE_PAGE_MANAGER))
#define GIS_PAGE_MANAGER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GIS_TYPE_PAGE_MANAGER, GisPageManagerClass))

typedef struct _GisPageManager        GisPageManager;
typedef struct _GisPageManagerClass   GisPageManagerClass;
typedef struct _GisPageManagerPrivate GisPageManagerPrivate;

struct _GisPageManager
{
	GObject          __parent__;

	GisPageManagerPrivate *priv;
};

struct _GisPageManagerClass
{
	GObjectClass __parent_class__;

	void (*go_next)        (GisPageManager *manager);
	void (*locale_changed) (GisPageManager *manager);
};


GType           gis_page_manager_get_type     (void);

GisPageManager *gis_page_manager_new          (void);

void            gis_page_manager_go_next      (GisPageManager *manager);

void            gis_page_manager_locale_changed (GisPageManager *manager);

void            gis_page_manager_set_user_info (GisPageManager *manager,
                                                const char     *realname,
                                                const char     *username,
                                                const char     *password);

void            gis_page_manager_get_user_info (GisPageManager  *manager,
                                                char           **realname,
                                                char           **username,
                                                char           **password);

void            gis_page_manager_set_network_available (GisPageManager *manager,
                                                        gboolean network_available);
gboolean        gis_page_manager_get_network_available (GisPageManager *manager);

void            gis_page_manager_set_online_accounts (GisPageManager *manager,
                                                      GList          *online_accounts);
GList          *gis_page_manager_get_online_accounts (GisPageManager *manager);

void            gis_page_manager_set_language (GisPageManager *manager,
                                               const char     *language);
char           *gis_page_manager_get_language (GisPageManager *manager);


G_END_DECLS

#endif /* __GIS_PAGE_MANAGER_H__ */
