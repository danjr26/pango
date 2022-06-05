/* Pango
 * pango-font-metrics.h: Font handling
 *
 * Copyright (C) 2000 Red Hat Software
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#pragma once

#include <pango/pango-types.h>
#include <pango/pango-font-description.h>

#include <glib-object.h>
#include <hb.h>

G_BEGIN_DECLS

/**
 * PangoFontMetrics:
 *
 * A `PangoFontMetrics` structure holds the overall metric information
 * for a font.
 *
 * The information in a `PangoFontMetrics` structure may be restricted
 * to a script. The fields of this structure are private to implementations
 * of a font backend. See the documentation of the corresponding getters
 * for documentation of their meaning.
 *
 * For an overview of the most important metrics, see:
 *
 * <picture>
 *   <source srcset="fontmetrics-dark.png" media="(prefers-color-scheme: dark)">
 *   <img alt="Font metrics" src="fontmetrics-light.png">
 * </picture>

 */
typedef struct _PangoFontMetrics PangoFontMetrics;

#define PANGO_TYPE_FONT_METRICS  (pango_font_metrics_get_type ())

struct _PangoFontMetrics
{
  /* <private> */
  guint ref_count;

  int ascent;
  int descent;
  int height;
  int approximate_char_width;
  int approximate_digit_width;
  int underline_position;
  int underline_thickness;
  int strikethrough_position;
  int strikethrough_thickness;
};

PANGO_AVAILABLE_IN_ALL
GType             pango_font_metrics_get_type                    (void) G_GNUC_CONST;
PANGO_AVAILABLE_IN_ALL
PangoFontMetrics *pango_font_metrics_ref                         (PangoFontMetrics *metrics);
PANGO_AVAILABLE_IN_ALL
void              pango_font_metrics_unref                       (PangoFontMetrics *metrics);
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_ascent                  (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_descent                 (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_height                  (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_approximate_char_width  (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_approximate_digit_width (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_underline_position      (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_underline_thickness     (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_strikethrough_position  (PangoFontMetrics *metrics) G_GNUC_PURE;
PANGO_AVAILABLE_IN_ALL
int               pango_font_metrics_get_strikethrough_thickness (PangoFontMetrics *metrics) G_GNUC_PURE;


G_END_DECLS
