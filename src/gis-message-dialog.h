/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
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
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef __GIS_MESSAGE_DIALOG_H__
#define __GIS_MESSAGE_DIALOG_H__

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GIS_TYPE_MESSAGE_DIALOG            (gis_message_dialog_get_type ())
#define GIS_MESSAGE_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), GIS_TYPE_MESSAGE_DIALOG, GisMessageDialog))
#define GIS_MESSAGE_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  GIS_TYPE_MESSAGE_DIALOG, GisMessageDialogClass))
#define GIS_IS_MESSAGE_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), GIS_TYPE_MESSAGE_DIALOG))
#define GIS_IS_MESSAGE_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE((klass),  GIS_TYPE_MESSAGE_DIALOG))
#define GIS_MESSAGE_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  GIS_TYPE_MESSAGE_DIALOG, GisMessageDialogClass))

typedef struct _GisMessageDialog GisMessageDialog;
typedef struct _GisMessageDialogClass GisMessageDialogClass;
typedef struct _GisMessageDialogPrivate GisMessageDialogPrivate;

struct _GisMessageDialog {
	GtkDialog dialog;

	GisMessageDialogPrivate *priv;
};

struct _GisMessageDialogClass {
	GtkDialogClass parent_class;
};

GType	    gis_message_dialog_get_type	   (void) G_GNUC_CONST;

GtkWidget  *gis_message_dialog_new		   (GtkWindow  *parent,
                                            const char *icon,
                                            const char *title,
                                            const char *message); 

void        gis_message_dialog_set_title   (GisMessageDialog *dialog,
                                            const char       *title);
void        gis_message_dialog_set_message (GisMessageDialog *dialog,
                                            const char       *message);
void        gis_message_dialog_set_icon    (GisMessageDialog *dialog,
                                            const char       *icon);

G_END_DECLS

#endif /* __GIS_MESSAGE_DIALOG_H__ */
