/*
 * Copyright (C) 2000 Red Hat Software
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdio.h>
#include <glib.h>
#include <pango/pango-font.h>
#include <pango/pango-direction.h>

G_BEGIN_DECLS

gboolean _pango_scan_int                (const char **pos,
                                         int         *out);

gboolean _pango_parse_enum              (GType       type,
                                         const char *str,
                                         int        *value,
                                         gboolean    warn,
                                         char      **possible_values);
gboolean pango_parse_flags              (GType       type,
                                         const char *str,
                                         int        *value,
                                         char      **possible_values);

char    *_pango_trim_string             (const char *str);

PangoDirection  pango_find_base_dir     (const char *text,
                                         int         length);

gboolean pango_parse_style              (const char   *str,
                                         PangoStyle   *style,
                                         gboolean      warn);
gboolean pango_parse_variant            (const char   *str,
                                         PangoVariant *variant,
                                         gboolean      warn);
gboolean pango_parse_weight             (const char   *str,
                                         PangoWeight  *weight,
                                         gboolean      warn);
gboolean pango_parse_stretch            (const char   *str,
                                         PangoStretch *stretch,
                                         gboolean      warn);

void     pango_quantize_line_geometry   (int *thickness,
                                         int *position);


G_END_DECLS
