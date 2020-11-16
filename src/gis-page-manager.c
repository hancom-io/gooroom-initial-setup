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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <locale.h>

#include "gis-page-manager.h"


enum {
	GO_NEXT,
	LOCALE_CHANGED,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];


struct _GisPageManagerPrivate {
	gchar *username;
	gchar *realname;
	gchar *password;
	gchar *language;

	gboolean network_available;

	GList *online_accounts;
};

G_DEFINE_TYPE_WITH_PRIVATE (GisPageManager, gis_page_manager, G_TYPE_OBJECT)

enum
{
	PROP_0,
	PROP_NETWORK_AVAILABLE,
	PROP_LAST,
};

static GParamSpec *obj_props[PROP_LAST];


static void
gis_page_manager_finalize (GObject *object)
{
	GisPageManager *manager = GIS_PAGE_MANAGER (object);
	GisPageManagerPrivate *priv = manager->priv;

	g_free (priv->realname);
	g_free (priv->username);
	g_free (priv->password);
	g_free (priv->language);

	if (priv->online_accounts) {
		g_list_free_full (priv->online_accounts, (GDestroyNotify) g_free);
		priv->online_accounts = NULL;
	}

	G_OBJECT_CLASS (gis_page_manager_parent_class)->finalize (object);
}

static void
gis_page_manager_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
	GisPageManager *manager = GIS_PAGE_MANAGER (object);
	GisPageManagerPrivate *priv = manager->priv;

	switch (prop_id)
	{
		case PROP_NETWORK_AVAILABLE:
			g_value_set_boolean (value, priv->network_available);
		break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gis_page_manager_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
	GisPageManager *manager = GIS_PAGE_MANAGER (object);
	GisPageManagerPrivate *priv = manager->priv;

	switch (prop_id)
	{
		case PROP_NETWORK_AVAILABLE:
			gis_page_manager_set_network_available (manager, g_value_get_boolean (value));
		break;
		default:
			G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
gis_page_manager_init (GisPageManager *manager)
{
	manager->priv = gis_page_manager_get_instance_private (manager);

	manager->priv->realname = NULL;
	manager->priv->username = NULL;
	manager->priv->password = NULL;
	manager->priv->language = NULL;

	manager->priv->network_available = FALSE;
}

static void
gis_page_manager_class_init (GisPageManagerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	object_class->finalize = gis_page_manager_finalize;
	object_class->get_property = gis_page_manager_get_property;
	object_class->set_property = gis_page_manager_set_property;

	obj_props[PROP_NETWORK_AVAILABLE] =
		g_param_spec_boolean ("network-available", "", "", FALSE,
				G_PARAM_STATIC_STRINGS | G_PARAM_READWRITE);

    signals[GO_NEXT] = g_signal_new ("go-next",
                                     GIS_TYPE_PAGE_MANAGER,
                                     G_SIGNAL_RUN_FIRST,
                                     G_STRUCT_OFFSET (GisPageManagerClass, go_next),
                                     NULL, NULL,
                                     g_cclosure_marshal_VOID__VOID,
                                     G_TYPE_NONE, 0);

    signals[LOCALE_CHANGED] = g_signal_new ("locale-changed",
                                            GIS_TYPE_PAGE_MANAGER,
                                            G_SIGNAL_RUN_FIRST,
                                            G_STRUCT_OFFSET (GisPageManagerClass, locale_changed),
                                            NULL, NULL,
                                            g_cclosure_marshal_VOID__VOID,
                                            G_TYPE_NONE, 0);

	g_object_class_install_properties (object_class, PROP_LAST, obj_props);
}

GisPageManager *
gis_page_manager_new (void)
{
	GObject *result;

	result = g_object_new (GIS_TYPE_PAGE_MANAGER, NULL);

	return GIS_PAGE_MANAGER (result);
}

void
gis_page_manager_go_next (GisPageManager *manager)
{
	g_signal_emit (G_OBJECT (manager), signals[GO_NEXT], 0);
}

void
gis_page_manager_locale_changed (GisPageManager *manager)
{
	g_signal_emit (G_OBJECT (manager), signals[LOCALE_CHANGED], 0);
}

void
gis_page_manager_set_user_info (GisPageManager *manager,
                                const char     *realname,
                                const char     *username,
                                const char     *password)
{
	GisPageManagerPrivate *priv = manager->priv;

	g_free (priv->realname);
	g_free (priv->username);
	g_free (priv->password);

	priv->realname = g_strdup (realname);
	priv->username = g_strdup (username);
	priv->password = g_strdup (password);
}

void
gis_page_manager_get_user_info (GisPageManager  *manager,
                                char           **realname,
                                char           **username,
                                char           **password)
{
	GisPageManagerPrivate *priv = manager->priv;

	if (realname)
		*realname = g_strdup (priv->realname);

	if (username)
		*username = g_strdup (priv->username);

	if (password)
		*password = g_strdup (priv->password);
}

void
gis_page_manager_set_network_available (GisPageManager *manager,
                                        gboolean        available)
{
	manager->priv->network_available = available;

	g_object_notify_by_pspec (G_OBJECT (manager), obj_props[PROP_NETWORK_AVAILABLE]);
}

gboolean
gis_page_manager_get_network_available (GisPageManager *manager)
{
	return manager->priv->network_available;
}

void
gis_page_manager_set_online_accounts (GisPageManager *manager,
                                      GList          *online_accounts)
{
	GisPageManagerPrivate *priv = manager->priv;

	if (priv->online_accounts) {
		g_list_free_full (priv->online_accounts, (GDestroyNotify) g_free);
		priv->online_accounts = NULL;
	}

	manager->priv->online_accounts = online_accounts;
}

GList *
gis_page_manager_get_online_accounts (GisPageManager *manager)
{
	return manager->priv->online_accounts;
}

void
gis_page_manager_set_language (GisPageManager *manager,
                               const char     *language)
{
	if (manager->priv->language)
		g_free (manager->priv->language);

	manager->priv->language = g_strdup (language);
}

gchar *
gis_page_manager_get_language (GisPageManager *manager)
{
	return (manager->priv->language ? g_strdup (manager->priv->language) : NULL);
}
