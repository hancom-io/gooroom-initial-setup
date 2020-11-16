/*
 * Copyright 2009-2010  Red Hat, Inc,
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 *
 * Written by: Matthias Clasen <mclasen@redhat.com>
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <utmp.h>

#include <glib.h>
#include <glib/gi18n.h>

#include "um-utils.h"

void
set_entry_validation_checkmark (GtkEntry *entry)
{
	g_object_set (entry, "caps-lock-warning", FALSE, NULL);

	gtk_entry_set_icon_from_icon_name (entry, GTK_ENTRY_ICON_SECONDARY, "object-select-symbolic");
}

void
set_entry_validation_error (GtkEntry    *entry,
                            const gchar *text)
{
	g_object_set (entry, "caps-lock-warning", FALSE, NULL);

	gtk_entry_set_icon_from_icon_name (entry, GTK_ENTRY_ICON_SECONDARY, "dialog-warning-symbolic");
	gtk_entry_set_icon_tooltip_text (entry, GTK_ENTRY_ICON_SECONDARY, text);
}

void
clear_entry_validation_error (GtkEntry *entry)
{
	gboolean warning;

	g_object_get (entry, "caps-lock-warning", &warning, NULL);

	if (warning)
		return;

	gtk_entry_set_icon_from_pixbuf (entry, GTK_ENTRY_ICON_SECONDARY, NULL);
	g_object_set (entry, "caps-lock-warning", TRUE, NULL);
}

#define MAXNAMELEN  (UT_NAMESIZE - 1)

static gboolean
is_username_used (const gchar *username)
{
	struct passwd *pwent;

	if (username == NULL || username[0] == '\0') {
		return FALSE;
	}

	pwent = getpwnam (username);

	return pwent != NULL;
}

gboolean
is_valid_realname (const gchar *name)
{
	return TRUE;

#if 0
	gboolean is_empty = TRUE;
	const gchar *c;

	/* Valid names must contain:
	 *   1) at least one character.
	 *   2) at least one non-"space" character.
	 */
	for (c = name; *c; c++) {
		gunichar unichar;

		unichar = g_utf8_get_char_validated (c, -1);

		/* Partial UTF-8 sequence or end of string */
		if (unichar == (gunichar) -1 || unichar == (gunichar) -2)
			break;

		/* Check for non-space character */
		if (!g_unichar_isspace (unichar)) {
			is_empty = FALSE;
			break;
		}
	}

	return !is_empty;
#endif
}

gboolean
is_valid_username (const gchar *username, gchar **tip)
{
	gboolean empty;
	gboolean in_use;
	gboolean too_long;
	const gchar *c;

	if (username == NULL || username[0] == '\0') {
		empty = TRUE;
		in_use = FALSE;
		too_long = FALSE;
	} else {
		empty = FALSE;
		in_use = is_username_used (username);
		too_long = strlen (username) > MAXNAMELEN;
	}

	if (empty)
		return FALSE;

	if (in_use) {
		*tip = g_strdup (_("Sorry, that user name isn’t available. Please try another."));
		return FALSE;
	}

	if (too_long) {
		*tip = g_strdup_printf (_("The username is too long."));
		return FALSE;
	}

	if (username[0] == '-') {
		*tip = g_strdup (_("The username cannot start with a “-”."));
		return FALSE;
	}

	/* First char must be a letter, and it must only composed
	 * of ASCII letters, digits, and a '.', '-', '_'
	 */
	for (c = username; *c; c++) {
		if (!((*c >= 'a' && *c <= 'z') ||
              (*c >= 'A' && *c <= 'Z') ||
              (*c >= '0' && *c <= '9') ||
              (*c == '_') || (*c == '.') ||
              (*c == '-' && c != username))) {
			*tip = g_strdup (_("The username should only consist of upper and lower case letters from a-z, digits and the following characters: . - _"));
			return FALSE;
		}
	}

	return TRUE;
}
