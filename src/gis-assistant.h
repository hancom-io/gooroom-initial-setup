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
 * Written by:
 *     Jasper St. Pierre <jstpierre@mecheye.net>
 */

#ifndef __GIS_ASSISTANT_H__
#define __GIS_ASSISTANT_H__

#include <gtk/gtk.h>

#include "gis-page.h"

G_BEGIN_DECLS

#define GIS_TYPE_ASSISTANT         (gis_assistant_get_type ())
#define GIS_ASSISTANT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GIS_TYPE_ASSISTANT, GisAssistant))
#define GIS_ASSISTANT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k),  GIS_TYPE_ASSISTANT, GisAssistantClass))
#define GIS_IS_ASSISTANT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GIS_TYPE_ASSISTANT))
#define GIS_IS_ASSISTANT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k),  GIS_TYPE_ASSISTANT))
#define GIS_ASSISTANT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o),  GIS_TYPE_ASSISTANT, GisAssistantClass))

typedef struct _GisAssistantPrivate GisAssistantPrivate;
typedef struct _GisAssistant        GisAssistant;
typedef struct _GisAssistantClass   GisAssistantClass;

struct _GisAssistant
{
	GtkBox __parent__;

	GisAssistantPrivate *priv;
};

struct _GisAssistantClass
{
	GtkBoxClass __parent_class__;
};


GType        gis_assistant_get_type          (void) G_GNUC_CONST;

GtkWidget   *gis_assistant_new               (void);

void         gis_assistant_add_page          (GisAssistant *assistant,
                                              GisPage      *page);

void         gis_assistant_save_data         (GisAssistant *assistant);
void         gis_assistant_next_page         (GisAssistant *assistant);
void         gis_assistant_prev_page         (GisAssistant *assistant);
void         gis_assistant_locale_changed    (GisAssistant *assistant);

const gchar *gis_assistant_get_title         (GisAssistant *assistant);
GisPage     *gis_assistant_get_current_page  (GisAssistant *assistant);

G_END_DECLS

#endif /* __GIS_ASSISTANT_H__ */
