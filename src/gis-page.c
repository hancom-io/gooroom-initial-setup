/*
 * Copyright (C) 2012 Red Hat
 * Copyright (C) 2015 - 2020 Gooroom <gooroom@gooroom.kr>
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

#include <glib-object.h>

#include "gis-page.h"
#include "gis-page-manager.h"

struct _GisPagePrivate
{
	char *title;

	guint complete : 1;
	guint skippable : 1;

	GisPageManager *manager;
};

G_DEFINE_ABSTRACT_TYPE_WITH_PRIVATE (GisPage, gis_page, GTK_TYPE_BIN);

enum
{
	PROP_0,
	PROP_MANAGER,
	PROP_TITLE,
	PROP_COMPLETE,
	PROP_SKIPPABLE,
	PROP_LAST,
};

static GParamSpec *obj_props[PROP_LAST];

static void
gis_page_get_property (GObject    *object,
                       guint       prop_id,
                       GValue     *value,
                       GParamSpec *pspec)
{
	GisPage *page = GIS_PAGE (object);
	GisPagePrivate *priv = page->priv;

	switch (prop_id)
	{
		case PROP_MANAGER:
			g_value_set_pointer (value, (gpointer) page->manager);
		break;
		case PROP_TITLE:
			g_value_set_string (value, priv->title);
		break;
		case PROP_COMPLETE:
			g_value_set_boolean (value, priv->complete);
		break;
		case PROP_SKIPPABLE:
			g_value_set_boolean (value, priv->skippable);
		break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gis_page_set_property (GObject      *object,
                       guint         prop_id,
                       const GValue *value,
                       GParamSpec   *pspec)
{
	GisPage *page = GIS_PAGE (object);
	GisPagePrivate *priv = page->priv;

	switch (prop_id)
	{
		case PROP_MANAGER:
			page->manager = g_value_get_pointer (value);
		break;
		case PROP_TITLE:
			gis_page_set_title (page, (char *) g_value_get_string (value));
		break;
		case PROP_COMPLETE:
			priv->complete = g_value_get_boolean (value);
		break;
		case PROP_SKIPPABLE:
			priv->skippable = g_value_get_boolean (value);
		break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gis_page_finalize (GObject *object)
{
	GisPage *page = GIS_PAGE (object);
	GisPagePrivate *priv = page->priv;

	g_free (priv->title);

	G_OBJECT_CLASS (gis_page_parent_class)->finalize (object);
}

static void
gis_page_dispose (GObject *object)
{
	G_OBJECT_CLASS (gis_page_parent_class)->dispose (object);
}

static void
gis_page_constructed (GObject *object)
{
	G_OBJECT_CLASS (gis_page_parent_class)->constructed (object);
}

static void
gis_page_class_init (GisPageClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->constructed = gis_page_constructed;
	object_class->dispose = gis_page_dispose;
	object_class->finalize = gis_page_finalize;
	object_class->get_property = gis_page_get_property;
	object_class->set_property = gis_page_set_property;

	obj_props[PROP_MANAGER] =
		g_param_spec_pointer ("manager", "", "",
				G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY);
	obj_props[PROP_TITLE] =
		g_param_spec_string ("title", "", "", "",
				G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
	obj_props[PROP_COMPLETE] =
		g_param_spec_boolean ("complete", "", "", FALSE,
				G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);
	obj_props[PROP_SKIPPABLE] =
		g_param_spec_boolean ("skippable", "", "", FALSE,
				G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);

	g_object_class_install_properties (object_class, PROP_LAST, obj_props);
}

static void
gis_page_init (GisPage *page)
{
	page->priv = gis_page_get_instance_private (page);
}

char *
gis_page_get_title (GisPage *page)
{
	GisPagePrivate *priv = page->priv;

	if (priv->title != NULL)
		return priv->title;
	else
		return "";
}

void
gis_page_set_title (GisPage *page, char *title)
{
	GisPagePrivate *priv = page->priv;

	g_clear_pointer (&priv->title, g_free);
	priv->title = g_strdup (title);

	g_object_notify_by_pspec (G_OBJECT (page), obj_props[PROP_TITLE]);
}

gboolean
gis_page_get_complete (GisPage *page)
{
	return page->priv->complete;
}

void
gis_page_set_complete (GisPage *page, gboolean complete)
{
	page->priv->complete = complete;

	g_object_notify_by_pspec (G_OBJECT (page), obj_props[PROP_COMPLETE]);
}

gboolean
gis_page_get_skippable (GisPage *page)
{
	return page->priv->skippable;
}

void
gis_page_set_skippable (GisPage *page, gboolean skippable)
{
	page->priv->skippable = skippable;

	g_object_notify_by_pspec (G_OBJECT (page), obj_props[PROP_SKIPPABLE]);
}

void
gis_page_shown (GisPage *page)
{
	if (GIS_PAGE_GET_CLASS (page)->shown)
		GIS_PAGE_GET_CLASS (page)->shown (page);
}

gboolean
gis_page_pre_next (GisPage *page)
{
	if (GIS_PAGE_GET_CLASS (page)->pre_next)
		return GIS_PAGE_GET_CLASS (page)->pre_next (page);

	return TRUE;
}

gboolean
gis_page_should_show (GisPage *page)
{
	if (GIS_PAGE_GET_CLASS (page)->should_show)
		return GIS_PAGE_GET_CLASS (page)->should_show (page);

	return TRUE;
}

void
gis_page_save_data (GisPage *page)
{
	if (GIS_PAGE_GET_CLASS (page)->save_data)
		GIS_PAGE_GET_CLASS (page)->save_data (page);
}

void
gis_page_locale_changed (GisPage *page)
{
	if (GIS_PAGE_GET_CLASS (page)->locale_changed)
		GIS_PAGE_GET_CLASS (page)->locale_changed (page);
}
