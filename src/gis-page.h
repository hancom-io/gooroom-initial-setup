/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
 * Copyright (C) 2012 Red Hat
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Written by:
 *     Jasper St. Pierre <jstpierre@mecheye.net>
 */

#ifndef __GIS_PAGE_H__
#define __GIS_PAGE_H__

#include <gtk/gtk.h>

#include "gis-page-manager.h"

G_BEGIN_DECLS

#define GIS_TYPE_PAGE            (gis_page_get_type ())
#define GIS_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIS_TYPE_PAGE, GisPage))
#define GIS_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIS_TYPE_PAGE, GisPageClass))
#define GIS_IS_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIS_TYPE_PAGE))
#define GIS_IS_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIS_TYPE_PAGE))
#define GIS_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIS_TYPE_PAGE, GisPageClass))

typedef struct _GisPage        GisPage;
typedef struct _GisPageClass   GisPageClass;
typedef struct _GisPagePrivate GisPagePrivate;

struct _GisPage
{
	GtkBin __parent__;

	GisPageManager *manager;

	GisPagePrivate *priv;
};

struct _GisPageClass
{
	GtkBinClass __parent_class__;

	void      (*shown)          (GisPage  *page);
	void      (*save_data)      (GisPage  *page);
	void      (*locale_changed) (GisPage  *page);
	gboolean  (*should_show)    (GisPage  *page);
	gboolean  (*pre_next)       (GisPage  *page);
};

GType gis_page_get_type (void);

char *       gis_page_get_title        (GisPage *page);
void         gis_page_set_title        (GisPage *page,
                                        char    *title);

gboolean     gis_page_get_complete     (GisPage *page);
void         gis_page_set_complete     (GisPage *page,
                                        gboolean complete);

gboolean     gis_page_get_skippable    (GisPage *page);
void         gis_page_set_skippable    (GisPage *page,
                                        gboolean skippable);

void         gis_page_shown            (GisPage  *page);
gboolean     gis_page_should_show      (GisPage  *page);
gboolean     gis_page_pre_next         (GisPage  *page);
gboolean     gis_page_is_done          (GisPage  *page,
                                        char    **error_message);

void         gis_page_save_data        (GisPage *page);
void         gis_page_locale_changed   (GisPage *page);

G_END_DECLS

#endif /* __GIS_PAGE_H__ */
