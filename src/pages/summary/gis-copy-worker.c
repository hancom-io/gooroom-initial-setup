/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
 *
 * Originally based on code from gnome-initial-setup-copy-worker.c of gnome-initial-setup project
 * (there was no relevant copyright header at the time).
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

#include <pwd.h>
#include <string.h>
#include <gio/gio.h>
#include <stdlib.h>
#include <stdio.h>

#define	INITIAL_SETUP_USER	"gis"

static gchar *user = NULL;


static GOptionEntry option_entries[] =
{
	{ "username",     'u', 0, G_OPTION_ARG_STRING, &user,     NULL, NULL },
    { NULL }
};

static char *
get_home_dir (const char *user)
{
	struct passwd pw, *pwp;
	char buf[4096] = {0,};

	getpwnam_r (user, &pw, buf, sizeof (buf), &pwp);
	if (pwp != NULL)
		return g_strdup (pwp->pw_dir);
	else
		return NULL;
}

static void
change_owner (const char *user_homedir)
{
	guint i = 0;
	static char *dirs[] = { ".xsessionrc", ".config", ".local", NULL };

	for (i = 0; dirs[i] != NULL; i++) {
		char *cmd = g_strdup_printf ("/bin/chown -R %s:%s %s/%s", user, user, user_homedir, dirs[i]);
		if (!g_spawn_command_line_sync (cmd, NULL, NULL, NULL, NULL)) {
		}
		g_free (cmd);
	}
}

static void
move_file_from_homedir (const char *user,
                        GFile      *src_base,
                        GFile      *dst_base,
                        const char *path)
{
	GError *error = NULL;
	GFile *src = g_file_get_child (src_base, path);
	GFile *dst = g_file_get_child (dst_base, path);
	GFile *dst_parent = g_file_get_parent (dst);
	char* src_path = g_file_get_path (src);
	char* dst_path = g_file_get_path (dst);
	char* dst_parent_path = g_file_get_path (dst_parent);

	g_file_make_directory_with_parents (dst_parent, NULL, NULL);

	if (!g_file_move (src, dst, G_FILE_COPY_NONE, NULL, NULL, NULL, &error)) {
		if (!g_error_matches (error, G_IO_ERROR, G_IO_ERROR_NOT_FOUND)) {
			g_warning ("Unable to move %s to %s: %s", src_path, dst_path, error->message);
		}
	}

	g_object_unref (src);
	g_object_unref (dst);
	g_object_unref (dst_parent);
	g_free (src_path);
	g_free (dst_path);
	g_free (dst_parent_path);
}

static gboolean
is_valid_username (const char *user)
{
	struct passwd pw, *pwp;
	char buf[4096] = {0,};

	getpwnam_r (user, &pw, buf, sizeof (buf), &pwp);

	return (pwp != NULL);
}

int
main (int argc, char **argv)
{
	GFile *src;
	GFile *dst;
	gint ret = 0;
	GError *error = NULL;
	gchar *initial_setup_homedir, *user_homedir;
	gboolean retval;
	GOptionContext *context;

	context = g_option_context_new (NULL);
	g_option_context_add_main_entries (context, option_entries, NULL);
	retval = g_option_context_parse (context, &argc, &argv, &error);
	g_option_context_free (context);

	/* parse options */
	if (!retval) {
		g_warning ("%s", error->message);
		g_error_free (error);
		ret = 1;
		goto done;
	}

	if (!user) {
		g_warning ("No user was specified.");
		ret = 2;
		goto done;
	}

	if (!is_valid_username (user)) {
		g_warning ("Invalid user");
		ret = 3;
		goto done;
	}

	initial_setup_homedir = get_home_dir (INITIAL_SETUP_USER);
	if (initial_setup_homedir == NULL) {
		g_warning ("No initial setup home directory");
		ret = 4;
		goto done;
	}

	src = g_file_new_for_path (initial_setup_homedir);
	if (!g_file_query_exists (src, NULL)) {
		g_warning ("No initial setup home directory");
		ret = 5;
		goto done;
	}

	user_homedir = get_home_dir (user);
	dst = g_file_new_for_path (user_homedir);

#define FILE(path) \
	move_file_from_homedir (user, src, dst, path);

	FILE (".xsessionrc");
	FILE (".config/user_agreements");
	FILE (".config/goa-1.0/accounts.conf");
	FILE (".local/share/keyrings/login.keyring");

	change_owner (user_homedir);

done:
	g_free (initial_setup_homedir);
	g_free (user_homedir);

	g_object_unref (src);
	g_object_unref (dst);

	return ret;
}
