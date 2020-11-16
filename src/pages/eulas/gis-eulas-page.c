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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "eulas-resources.h"
#include "gis-eulas-page.h"
#include "utils.h"

#include <glib/gi18n-lib.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

typedef enum {
	TEXT,
	MARKUP,
	SKIP,
} FileType;

struct _GisEulasPagePrivate {
	GtkWidget *checkbox;
	GtkWidget *scrolled_window;
	GtkWidget *text_view;

	GFile *eulas;

	gboolean require_checkbox;
};

G_DEFINE_TYPE_WITH_PRIVATE (GisEulasPage, gis_eulas_page, GIS_TYPE_PAGE);


static FileType
get_file_type (GFile *file)
{
	gchar *path, *last_dot;
	FileType type;

	path = g_file_get_path (file);
	last_dot = strrchr (path, '.');

	if (g_strcmp0 (last_dot, ".txt") == 0)
		type = TEXT;
	else if (g_strcmp0 (last_dot, ".xml") == 0)
		type = MARKUP;
	else
		type = SKIP;

	g_free (path);

	return type;
}

static gboolean
build_eulas_text_buffer (GFile          *file,
                         GtkTextBuffer **buffer_out,
                         GError        **error_out)
{
	GtkTextBuffer *buffer = NULL;
	GtkTextIter start, end;
	GError *error = NULL;
	GInputStream *input_stream = NULL;
	FileType type = get_file_type (file);

	if (type == SKIP)
		return FALSE;

	input_stream = G_INPUT_STREAM (g_file_read (file, NULL, &error));
	if (input_stream == NULL)
		goto error_out;

	buffer = gtk_text_buffer_new (NULL);

	switch (type)
	{
		case TEXT:
			if (!splice_buffer_text (input_stream, buffer, &error))
				goto error_out;

			/* monospace the text */
			gtk_text_buffer_create_tag (buffer, "monospace", "family", "monospace", NULL);
			gtk_text_buffer_get_start_iter (buffer, &start);
			gtk_text_buffer_get_end_iter (buffer, &end);
			gtk_text_buffer_apply_tag_by_name (buffer, "monospace", &start, &end);
		break;

		case MARKUP:
			if (!splice_buffer_markup (input_stream, buffer, &error))
				goto error_out;
		break;

		default:
		break;
	}

	*buffer_out = buffer;

	return TRUE;

error_out:
	g_propagate_error (error_out, error);

	if (buffer != NULL)
		g_object_unref (buffer);

	return FALSE;
}

static gchar *
get_language (GisEulasPage *page)
{
	gchar *lang = NULL;
	GisPageManager *manager = GIS_PAGE (page)->manager;

	lang = gis_page_manager_get_language (manager);

	if (!lang)
		lang = g_getenv ("LANG") ? g_strdup (g_getenv ("LANG")) : NULL;

	if (!lang) {
		gsize len, i;
		gchar *buffer = NULL;
		if (g_file_get_contents ("/etc/default/locale", &buffer, &len, NULL)) {
			for (i = 0; i < len; i++) {
				if (buffer[i] == '\n')
					buffer[i] = '\0';
			}
			gchar **items = g_strsplit (buffer, "=", -1);
			if (items && g_strv_length (items) > 1)
				lang = g_strdup (items[1]);

			g_strfreev (items);
		}
		g_free (buffer);
	}

	return lang;
}

static gboolean
get_page_complete (GisEulasPage *page)
{
	GisEulasPagePrivate *priv = page->priv;

	if (priv->require_checkbox) {
		GtkToggleButton *checkbox = GTK_TOGGLE_BUTTON (priv->checkbox);
		if (!gtk_toggle_button_get_active (checkbox)) {
			return FALSE;
		}
	}

	return TRUE;
}

static void
sync_page_complete (GisEulasPage *page)
{
	gis_page_set_complete (GIS_PAGE (page), get_page_complete (page));
}

static void
gis_eulas_page_constructed (GObject *object)
{
	gboolean require_checkbox = TRUE;

	GisEulasPage *self = GIS_EULAS_PAGE (object);
	GisEulasPagePrivate *priv = self->priv;

	G_OBJECT_CLASS (gis_eulas_page_parent_class)->constructed (object);

	gtk_text_view_set_border_window_size (GTK_TEXT_VIEW (priv->text_view), GTK_TEXT_WINDOW_TOP, 12);
	gtk_text_view_set_border_window_size (GTK_TEXT_VIEW (priv->text_view), GTK_TEXT_WINDOW_LEFT, 12);
	gtk_text_view_set_border_window_size (GTK_TEXT_VIEW (priv->text_view), GTK_TEXT_WINDOW_RIGHT, 12);
	gtk_text_view_set_border_window_size (GTK_TEXT_VIEW (priv->text_view), GTK_TEXT_WINDOW_BOTTOM, 12);

	priv->require_checkbox = require_checkbox;

	gtk_widget_set_visible (priv->checkbox, require_checkbox);

	if (require_checkbox) {
		g_signal_connect_swapped (G_OBJECT (priv->checkbox), "toggled",
                                  G_CALLBACK (sync_page_complete), self);
	}

	sync_page_complete (self);

	gtk_widget_show (GTK_WIDGET (self));
}

static void
gis_eulas_page_locale_changed (GisPage *page)
{
	GisEulasPage *self = GIS_EULAS_PAGE (page);

	gis_page_set_title (GIS_PAGE (page), _("License Agreements"));

	gtk_button_set_label (GTK_BUTTON (self->priv->checkbox),
                          _("I have _agreed to the terms and conditions in this end user license agreement."));
}

static void
gis_eulas_page_shown (GisPage *page)
{
	const gchar *eulas_name;
	gchar *eulas_dir_path, *lang;
	GisEulasPage *self = GIS_EULAS_PAGE (page);
	GisEulasPagePrivate *priv = self->priv;

	lang = get_language (self);
	if (g_str_equal (lang, "ko_KR.UTF-8")) {
		eulas_name = "user_agreements_ko.txt";
	} else {
		eulas_name = "user_agreements_en.txt";
	}

	eulas_dir_path = g_build_filename (PKGDATADIR, "eulas", eulas_name, NULL);
	priv->eulas = g_file_new_for_path (eulas_dir_path);
	g_free (eulas_dir_path);

	if (g_file_query_exists (priv->eulas, NULL)) {
		GError *error = NULL;
		GtkTextBuffer *buffer;

		if (!build_eulas_text_buffer (priv->eulas, &buffer, &error)) {
			if (error) {
				g_warning ("Error while reading EULAS: %s", error->message);
				g_clear_error (&error);
			}
		}

		gtk_text_view_set_buffer (GTK_TEXT_VIEW (priv->text_view), buffer);
	}
}

static void
gis_eulas_page_save_data (GisPage *page)
{
	GisEulasPage *self = GIS_EULAS_PAGE (page);
	GisEulasPagePrivate *priv = self->priv;

	if (priv->eulas) {
		if (g_file_query_exists (priv->eulas, NULL)) {
			GFile *dest = NULL;
			gchar *dest_path = NULL;
			GError *error = NULL;

			dest_path = g_build_filename (g_get_user_config_dir (), "user_agreements", NULL);
			dest = g_file_new_for_path (dest_path);
			if (!g_file_copy (priv->eulas, dest, G_FILE_COPY_OVERWRITE, NULL, NULL, NULL, &error)) {
				if (error) {
					g_warning ("Failed to copy user_agreements: %s", error->message);
					g_error_free (error);
				} else {
					g_warning ("Failed to copy user_agreements");
				}
			}
			g_free (dest_path);
			g_object_unref (dest);
		}
	}
}

static void
gis_eulas_page_dispose (GObject *object)
{
	GisEulasPage *self = GIS_EULAS_PAGE (object);

	g_clear_object (&self->priv->eulas);

	G_OBJECT_CLASS (gis_eulas_page_parent_class)->dispose (object);
}

static void
gis_eulas_page_init (GisEulasPage *page)
{
	GisEulasPagePrivate *priv;
  
	priv = page->priv = gis_eulas_page_get_instance_private (page);

	g_resources_register (eulas_get_resource ());

	gtk_widget_init_template (GTK_WIDGET (page));

	gis_page_set_title (GIS_PAGE (page), _("License Agreements"));
}

static void
gis_eulas_page_class_init (GisEulasPageClass *klass)
{
	GisPageClass *page_class = GIS_PAGE_CLASS (klass);
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
			"/kr/gooroom/initial-setup/pages/eulas/gis-eulas-page.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisEulasPage, checkbox);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisEulasPage, scrolled_window);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisEulasPage, text_view);

	page_class->shown = gis_eulas_page_shown;
	page_class->save_data = gis_eulas_page_save_data;
	page_class->locale_changed = gis_eulas_page_locale_changed;

	object_class->constructed = gis_eulas_page_constructed;
	object_class->dispose = gis_eulas_page_dispose;
}

GisPage *
gis_prepare_eulas_page (GisPageManager *manager)
{
	return g_object_new (GIS_TYPE_EULAS_PAGE,
                         "manager", manager,
                         NULL);
}
