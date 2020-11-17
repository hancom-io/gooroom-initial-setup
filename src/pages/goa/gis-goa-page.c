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

#include "gis-goa-page.h"
#include "goa-resources.h"

#define GOA_API_IS_SUBJECT_TO_CHANGE
#include <goa/goa.h>
#define GOA_BACKEND_API_IS_SUBJECT_TO_CHANGE
#include <goabackend/goabackend.h>

#include <gio/gio.h>
#include <glib/gi18n-lib.h>
#include <libsecret/secret.h>


struct _GisGoaPagePrivate {
	GtkWidget *subtitle_label;
	GtkWidget *accounts_list;
	GtkWidget *accounts_list_box;
	GtkWidget *error_box;
	GtkWidget *error_label;
	GtkWidget *error_image;

	GoaClient *goa_client;
	GHashTable *providers;
	gboolean accounts_exist;
};


G_DEFINE_TYPE_WITH_PRIVATE (GisGoaPage, gis_goa_page, GIS_TYPE_PAGE);

struct _ProviderWidget {
	GisGoaPage *page;
	GoaProvider *provider;
	GoaAccount *displayed_account;

	GtkWidget *row;
	GtkWidget *checkmark;
	GtkWidget *label;
	GtkWidget *account_label;
};
typedef struct _ProviderWidget ProviderWidget;


static void
sync_provider_widget (ProviderWidget *provider_widget)
{
	gboolean has_account = (provider_widget->displayed_account != NULL);

	gtk_widget_set_visible (provider_widget->checkmark, has_account);
	gtk_widget_set_visible (provider_widget->account_label, has_account);
	gtk_widget_set_sensitive (provider_widget->row, !has_account);

	if (has_account) {
		char *markup;
		markup = g_strdup_printf ("<small><span foreground=\"#555555\">%s</span></small>",
                                  goa_account_get_presentation_identity (provider_widget->displayed_account));
		gtk_label_set_markup (GTK_LABEL (provider_widget->account_label), markup);
		g_free (markup);

		const gchar *id;
		gchar *password = NULL;
		gchar *password_key = NULL;

		static const SecretSchema secret_password_schema =
		{
			"org.gnome.OnlineAccounts", SECRET_SCHEMA_DONT_MATCH_NAME,
			{
				{ "goa-identity", SECRET_SCHEMA_ATTRIBUTE_STRING },
				{ "NULL", 0 }
			}
		};

		id = goa_account_get_id (provider_widget->displayed_account);

		password_key = g_strdup_printf ("%s:gen%d:%s",
				goa_provider_get_provider_type (provider_widget->provider),
				goa_provider_get_credentials_generation (provider_widget->provider),
				id);

		password = secret_password_lookup_sync (&secret_password_schema,
				NULL,
				NULL,
				"goa-identity", password_key,
				NULL);
	}
}

static void
add_account_to_provider (ProviderWidget *provider_widget)
{
	GError *error = NULL;
	GtkWidget *dialog;
	GisGoaPage *page = provider_widget->page;
	GisGoaPagePrivate *priv = page->priv;
	GtkWindow *parent = GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET (page)));

	dialog = gtk_dialog_new_with_buttons (_("Add Account"),
                                          parent,
                                          GTK_DIALOG_MODAL
                                          | GTK_DIALOG_DESTROY_WITH_PARENT
                                          | GTK_DIALOG_USE_HEADER_BAR,
                                          NULL, NULL);

	goa_provider_add_account (provider_widget->provider,
                              priv->goa_client,
                              GTK_DIALOG (dialog),
                              GTK_BOX (gtk_dialog_get_content_area (GTK_DIALOG (dialog))),
                              &error);

  /* this will fire the `account-added` signal, which will do
   * the syncing of displayed_account on its own */

	if (error) {
		if (!g_error_matches (error, GOA_ERROR, GOA_ERROR_DIALOG_DISMISSED))
			g_warning ("fart %s", error->message);
		goto out;
	}

out:
	gtk_widget_destroy (dialog);
}

static void
add_provider_to_list (GisGoaPage *page, const char *provider_type)
{
	GtkWidget *row;
	GtkWidget *box;
	GtkWidget *image;
	GtkWidget *label;
	GtkWidget *checkmark;
	GtkWidget *account_label;
	GIcon *icon;
	gchar *markup, *provider_name;
	GoaProvider *provider;
	ProviderWidget *provider_widget;
	GisGoaPagePrivate *priv = page->priv;

	provider = goa_provider_get_for_provider_type (provider_type);
	if (provider == NULL)
		return;

	row = gtk_list_box_row_new ();
	box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL, 6);
	g_object_set (box, "margin", 4, NULL);
	gtk_widget_set_hexpand (box, TRUE);

	icon = goa_provider_get_provider_icon (provider, NULL);
	image = gtk_image_new_from_gicon (icon, GTK_ICON_SIZE_DIALOG);
	g_object_unref (icon);

	provider_name = goa_provider_get_provider_name (provider, NULL);
	markup = g_strdup_printf ("<b>%s</b>", provider_name);
	label = gtk_label_new (NULL);
	gtk_label_set_markup (GTK_LABEL (label), markup);
	g_free (markup);
	g_free (provider_name);

	checkmark = gtk_image_new_from_icon_name ("object-select-symbolic", GTK_ICON_SIZE_MENU);

	account_label = gtk_label_new (NULL);

	gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_box_pack_end (GTK_BOX (box), checkmark, FALSE, FALSE, 8);
	gtk_box_pack_end (GTK_BOX (box), account_label, FALSE, FALSE, 0);

	gtk_container_add (GTK_CONTAINER (row), box);

	gtk_widget_show (label);
	gtk_widget_show (image);
	gtk_widget_show (box);
	gtk_widget_show (row);

	provider_widget = g_new0 (ProviderWidget, 1);
	provider_widget->page = page;
	provider_widget->provider = provider;
	provider_widget->row = row;
	provider_widget->checkmark = checkmark;
	provider_widget->label = label;
	provider_widget->account_label = account_label;

	g_object_set_data (G_OBJECT (row), "widget", provider_widget);

	g_hash_table_insert (priv->providers, (char *) provider_type, provider_widget);

	gtk_container_add (GTK_CONTAINER (priv->accounts_list), row);
}

static void
populate_provider_list (GisGoaPage *page)
{
	add_provider_to_list (page, "google");
	add_provider_to_list (page, "owncloud");
	add_provider_to_list (page, "windows_live");
}

static void
sync_accounts (GisGoaPage *page)
{
	GList *accounts = NULL, *l = NULL, *online_accounts = NULL;
	GisGoaPagePrivate *priv = page->priv;

	accounts = goa_client_get_accounts (priv->goa_client);

	for (l = accounts; l != NULL; l = l->next) {
		GoaObject *object = GOA_OBJECT (l->data);
		GoaAccount *account = goa_object_get_account (object);
		const char *account_type = goa_account_get_provider_type (account);
		ProviderWidget *provider_widget;

		provider_widget = g_hash_table_lookup (priv->providers, account_type);
		if (!provider_widget)
			continue;

		priv->accounts_exist = TRUE;

		if (provider_widget->displayed_account)
			continue;

		provider_widget->displayed_account = account;
		sync_provider_widget (provider_widget);
	}

	g_list_free_full (accounts, (GDestroyNotify) g_object_unref);

	gis_page_set_skippable (GIS_PAGE (page), !priv->accounts_exist);
	gis_page_set_complete (GIS_PAGE (page), priv->accounts_exist);
}

static void
accounts_changed (GoaClient *client, GoaObject *object, gpointer user_data)
{
	GisGoaPage *page = GIS_GOA_PAGE (user_data);

	sync_accounts (page);
}

static void
network_available_changed_cb (GObject    *gobject,
                              GParamSpec *pspec,
                              gpointer    user_data)
{
	gboolean network_available;
	GisGoaPage *page = GIS_GOA_PAGE (user_data);
	GisGoaPagePrivate *priv = page->priv;
	GisPageManager *manager = GIS_PAGE (page)->manager;

	network_available = gis_page_manager_get_network_available (manager);

	if (network_available) {
		gtk_label_set_text (GTK_LABEL (priv->error_label), _("The system's network is inactive"));

		gtk_widget_hide (GTK_WIDGET (priv->error_box));
		gtk_widget_show (GTK_WIDGET (priv->accounts_list_box));
	} else {
		gtk_widget_show (GTK_WIDGET (priv->error_box));
		gtk_widget_hide (GTK_WIDGET (priv->accounts_list_box));
	}
}

static void
update_header_func (GtkListBoxRow *child,
                    GtkListBoxRow *before,
                    gpointer       user_data)
{
	GtkWidget *header;

	if (before == NULL)
		return;

	header = gtk_separator_new (GTK_ORIENTATION_HORIZONTAL);
	gtk_list_box_row_set_header (child, header);
	gtk_widget_show (header);
}

static void
row_activated (GtkListBox    *box,
               GtkListBoxRow *row,
               GisGoaPage    *page)
{
	ProviderWidget *provider_widget;

	if (row == NULL)
		return;

	provider_widget = g_object_get_data (G_OBJECT (row), "widget");
	g_assert (provider_widget != NULL);
	g_assert (provider_widget->displayed_account == NULL);

	add_account_to_provider (provider_widget);
}

static gboolean
foreach_metadata_free (gpointer key,
                       gpointer value,
                       gpointer user_data)
{
	ProviderWidget *provider_widget = (ProviderWidget *)value;

	g_object_unref (provider_widget->provider);

	g_free (provider_widget);
}

static void
gis_goa_page_finalize (GObject *object)
{
	GisGoaPage *self = GIS_GOA_PAGE (object);
	GisGoaPagePrivate *priv = self->priv;

	if (priv->providers) {
		g_hash_table_foreach_remove (priv->providers, foreach_metadata_free, self);
		g_hash_table_destroy (priv->providers);
	}

	G_OBJECT_CLASS (gis_goa_page_parent_class)->finalize (object);
}

static void
gis_goa_page_dispose (GObject *object)
{
	GisGoaPage *page = GIS_GOA_PAGE (object);
	GisGoaPagePrivate *priv = page->priv;

	g_clear_object (&priv->goa_client);

	G_OBJECT_CLASS (gis_goa_page_parent_class)->dispose (object);
}

static gboolean
gis_goa_page_pre_next (GisPage *page)
{
    GHashTableIter iter;
	GList *online_accounts = NULL;
	ProviderWidget *provider_widget = NULL;
	GisGoaPagePrivate *priv = GIS_GOA_PAGE (page)->priv;

	g_hash_table_iter_init (&iter, priv->providers);
	while (g_hash_table_iter_next (&iter, NULL, (gpointer) &provider_widget)) {
		if (provider_widget->displayed_account) {
			gchar *provider_name = goa_provider_get_provider_name (provider_widget->provider, NULL);
			online_accounts = g_list_append (online_accounts, g_strdup (provider_name));
			g_free (provider_name);
		}
	}

	gis_page_manager_set_online_accounts (page->manager, online_accounts);

	return TRUE;
}

static void
gis_goa_page_locale_changed (GisPage *page)
{
    GHashTableIter iter;
	ProviderWidget *provider_widget;

	GisGoaPagePrivate *priv = GIS_GOA_PAGE (page)->priv;

	gis_page_set_title (GIS_PAGE (page), _("Online Accounts Settings"));

	gtk_label_set_text (GTK_LABEL (priv->subtitle_label),
                        _("Connect your accounts to easily access your email, "
                          "online calendar, contacts, documents and photos. "
                          "Accounts can be added and removed at any time from the Settings application."));
	gtk_label_set_text (GTK_LABEL (priv->error_label), "The system's network is inactive");

	g_hash_table_iter_init (&iter, priv->providers);
	while (g_hash_table_iter_next (&iter, NULL, (gpointer) &provider_widget)) {
		gchar *provider_name = goa_provider_get_provider_name (provider_widget->provider, NULL);
		gchar *markup = g_strdup_printf ("<b>%s</b>", provider_name);
		gtk_label_set_markup (GTK_LABEL (provider_widget->label), markup);
		g_free (markup);
		g_free (provider_name);
	}
}

static gboolean
gis_goa_page_should_show (GisPage *page)
{
	gboolean should_show;
	GisPageManager *manager = page->manager;

	should_show = gis_page_manager_get_network_available (manager);

	return should_show;
}

static void
gis_goa_page_constructed (GObject *object)
{
	GError *error = NULL;
	GisGoaPage *page = GIS_GOA_PAGE (object);
	GisGoaPagePrivate *priv = page->priv;

	G_OBJECT_CLASS (gis_goa_page_parent_class)->constructed (object);

	gis_page_set_skippable (GIS_PAGE (page), TRUE);

	priv->goa_client = goa_client_new_sync (NULL, &error);
	if (priv->goa_client == NULL) {
		g_warning ("Failed to get a GoaClient: %s", error->message);
		g_error_free (error);
		gtk_label_set_text (GTK_LABEL (priv->error_label), "Internal Error");

		gtk_widget_show_all (GTK_WIDGET (priv->error_box));
		gtk_widget_hide (GTK_WIDGET (priv->accounts_list_box));
		return;
	}

	g_signal_connect (priv->goa_client, "account-added",
                      G_CALLBACK (accounts_changed), page);
	g_signal_connect (priv->goa_client, "account-removed",
                      G_CALLBACK (accounts_changed), page);

	g_signal_connect (GIS_PAGE (page)->manager, "notify::network-available",
                      G_CALLBACK (network_available_changed_cb), page);

	gtk_list_box_set_header_func (GTK_LIST_BOX (priv->accounts_list), update_header_func, NULL, NULL);
	g_signal_connect (priv->accounts_list, "row-activated", G_CALLBACK (row_activated), page);

	populate_provider_list (page);

	sync_accounts (page);

done:
	gtk_widget_show (GTK_WIDGET (page));
}

static void
gis_goa_page_init (GisGoaPage *page)
{
	GisGoaPagePrivate *priv;

	priv = page->priv = gis_goa_page_get_instance_private (page);

	priv->accounts_exist = FALSE;
	priv->providers = g_hash_table_new (g_str_hash, g_str_equal);

	g_resources_register (goa_get_resource ());

	gtk_widget_init_template (GTK_WIDGET (page));

	gis_page_set_title (GIS_PAGE (page), _("Online Accounts Settings"));
}

static void
gis_goa_page_class_init (GisGoaPageClass *klass)
{
	GisPageClass *page_class = GIS_PAGE_CLASS (klass);
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
			"/kr/gooroom/initial-setup/pages/goa/gis-goa-page.ui");

	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, subtitle_label);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, accounts_list);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, accounts_list_box);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, error_box);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, error_image);
	gtk_widget_class_bind_template_child_private (GTK_WIDGET_CLASS (klass), GisGoaPage, error_label);

	page_class->pre_next = gis_goa_page_pre_next;
	page_class->locale_changed = gis_goa_page_locale_changed;
	page_class->should_show  = gis_goa_page_should_show;

	object_class->constructed = gis_goa_page_constructed;
	object_class->dispose = gis_goa_page_dispose;
	object_class->finalize = gis_goa_page_finalize;
}

GisPage *
gis_prepare_goa_page (GisPageManager *manager)
{
	return g_object_new (GIS_TYPE_GOA_PAGE,
                         "manager", manager,
                         NULL);
}
