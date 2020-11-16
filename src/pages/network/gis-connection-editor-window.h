/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
 * Copyright (C) 2004-2005 William Jon McCann <mccann@jhu.edu>
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
 * Authors: William Jon McCann <mccann@jhu.edu>
 *
 */

#ifndef __GIS_CONNECTION_EDITOR_WINDOW_H__
#define __GIS_CONNECTION_EDITOR_WINDOW_H__

#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GIS_TYPE_CONNECTION_EDITOR_WINDOW         (gis_connection_editor_window_get_type ())
#define GIS_CONNECTION_EDITOR_WINDOW(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GIS_TYPE_CONNECTION_EDITOR_WINDOW, GisConnectionEditorWindow))
#define GIS_CONNECTION_EDITOR_WINDOW_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GIS_TYPE_CONNECTION_EDITOR_WINDOW, GisConnectionEditorWindowClass))
#define GIS_IS_CONNECTION_EDITOR_WINDOW(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GIS_TYPE_CONNECTION_EDITOR_WINDOW))
#define GIS_IS_CONNECTION_EDITOR_WINDOW_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GIS_TYPE_CONNECTION_EDITOR_WINDOW))
#define GIS_CONNECTION_EDITOR_WINDOW_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GIS_CONNECTION_EDITOR_WINDOW, GisConnectionEditorWindowClass))

typedef struct _GisConnectionEditorWindow        GisConnectionEditorWindow;
typedef struct _GisConnectionEditorWindowClass   GisConnectionEditorWindowClass;
typedef struct _GisConnectionEditorWindowPrivate GisConnectionEditorWindowPrivate;


struct _GisConnectionEditorWindow
{
	GtkWindow  __parent__;

	GisConnectionEditorWindowPrivate *priv;
};

struct _GisConnectionEditorWindowClass
{
	GtkWindowClass   __parent_class__;
};

GType                       gis_connection_editor_window_get_type (void);

GisConnectionEditorWindow  *gis_connection_editor_window_new (GdkMonitor *monitor,
                                                              gchar      *uuid);

G_END_DECLS

#endif /* __GIS_CONNECTION_EDITOR_WINDOW_H */
