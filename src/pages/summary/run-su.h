/*
 * Copyright (C) 2015-2020 Gooroom <gooroom@gooroom.kr>
 * Copyright (C) 2010 Milan Bouchet-Valat
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Milan Bouchet-Valat <nalimilan@club.fr>
 */

#ifndef __RUN_SU_H__
#define __RUN_SU_H__


struct SuHandler;

typedef struct SuHandler SuHandler;

typedef void (*SuCallback) (SuHandler *su_handler, GError *error, const gpointer user_data);

/* Error codes */
typedef enum {
	SU_ERROR_AUTH_FAILED,       /* Wrong old password, or PAM failure */
	SU_ERROR_BACKEND,           /* Backend error */
} SuError;


SuHandler *su_init            (void);

void       su_destroy         (SuHandler  *su_handler);

void       su_authenticate    (SuHandler  *su_handler,
                               const char *user,
                               const char *current_password,
                               SuCallback  cb,
                               gpointer    user_data);

G_END_DECLS

#endif /* __RUN_SU_H__ */

