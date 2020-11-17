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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <glib.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "gis-message-dialog.h"

struct _GisMessageDialogPrivate {
	GtkWidget *icon_image;
	GtkWidget *title_label;
	GtkWidget *message_label;
};

G_DEFINE_TYPE_WITH_PRIVATE (GisMessageDialog, gis_message_dialog, GTK_TYPE_DIALOG);


static void
gis_message_dialog_close (GtkDialog *dialog)
{
}

#if 0
static void
gis_message_dialog_finalize (GObject *object)
{
	GisMessageDialog *dialog = GIS_ARS_DIALOG (object);
	GisMessageDialogPrivate *priv = dialog->priv;

	G_OBJECT_CLASS (gis_message_dialog_parent_class)->finalize (object);
}
#endif

static void
gis_message_dialog_init (GisMessageDialog *dialog)
{
	dialog->priv = gis_message_dialog_get_instance_private (dialog);

	gtk_widget_init_template (GTK_WIDGET (dialog));

	gtk_window_set_decorated (GTK_WINDOW (dialog), FALSE);
	gtk_window_set_skip_taskbar_hint (GTK_WINDOW (dialog), TRUE);
	gtk_window_set_skip_pager_hint (GTK_WINDOW (dialog), TRUE);
	gtk_widget_set_app_paintable (GTK_WIDGET (dialog), TRUE);

	GdkScreen *screen = gtk_window_get_screen (GTK_WINDOW (dialog));
	if (gdk_screen_is_composited (screen)) {
		GdkVisual *visual = gdk_screen_get_rgba_visual (screen);
		if (visual == NULL)
			visual = gdk_screen_get_system_visual (screen);

		gtk_widget_set_visual (GTK_WIDGET (dialog), visual);
	}
}

static void
gis_message_dialog_class_init (GisMessageDialogClass *klass)
{
//	GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
	GtkDialogClass *dialog_class = GTK_DIALOG_CLASS (klass);

//	gobject_class->finalize = gis_message_dialog_finalize;

	dialog_class->close = gis_message_dialog_close;

	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                                 "/kr/gooroom/initial-setup/gis-message-dialog.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass),
                                                  GisMessageDialog, icon_image);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass),
                                                 GisMessageDialog, title_label);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass),
                                                 GisMessageDialog, message_label);
}

GtkWidget *
gis_message_dialog_new (GtkWindow *parent,
                        const char *icon,
                        const char *title,
                        const char *message)
{
	GisMessageDialog *dialog;

	dialog = GIS_MESSAGE_DIALOG (g_object_new (GIS_TYPE_MESSAGE_DIALOG,
                                               "use-header-bar", FALSE,
                                               "transient-for", parent,
                                               NULL));

	gis_message_dialog_set_icon (dialog, icon);
	gis_message_dialog_set_title (dialog, title);
	gis_message_dialog_set_message (dialog, message);

	return GTK_WIDGET (dialog);
}

void
gis_message_dialog_set_title (GisMessageDialog *dialog,
                              const char       *title)
{
	if (title)
		gtk_label_set_text (GTK_LABEL (dialog->priv->title_label), title);
}

void
gis_message_dialog_set_message (GisMessageDialog *dialog,
                                const char       *message)
{
	if (message)
		gtk_label_set_text (GTK_LABEL (dialog->priv->message_label), message);
}

void
gis_message_dialog_set_icon (GisMessageDialog *dialog,
                             const char       *icon)
{
	if (icon) {
		gtk_image_set_from_icon_name (GTK_IMAGE (dialog->priv->icon_image),
                                      icon, GTK_ICON_SIZE_DIALOG);

		gtk_widget_show (dialog->priv->icon_image);
	}
}
