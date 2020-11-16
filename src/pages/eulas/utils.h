/*
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
 */


#ifndef __UTILS_H__
#define __UTILS_H__

#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

gboolean splice_buffer_text (GInputStream  *stream,
                             GtkTextBuffer *buffer,
                             GError       **error);

gboolean splice_buffer_markup (GInputStream  *stream,
                               GtkTextBuffer *buffer,
                               GError       **error);

G_END_DECLS

#endif /* __UTILS_H__ */
