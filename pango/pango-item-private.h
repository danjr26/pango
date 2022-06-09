/*
 * Copyright 2021 Red Hat, Inc.
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

#include <pango/pango-item.h>
#include <pango/pango-break.h>

/*< private >
 * PangoAnalysis:
 * @size_font: font to use for determining line height
 * @font: the font for this segment
 * @level: the bidirectional level for this segment.
 * @gravity: the glyph orientation for this segment (A `PangoGravity`).
 * @flags: boolean flags for this segment
 * @script: the detected script for this segment (A `PangoScript`)
 * @language: the detected language for this segment.
 * @extra_attrs: extra attributes for this segment.
 *
 * The `PangoAnalysis` structure stores information about
 * the properties of a segment of text.
 */
struct _PangoAnalysis
{
  PangoFont *size_font;
  PangoFont *font;

  guint8 level;
  guint8 gravity;
  guint8 flags;

  guint8 script;
  PangoLanguage *language;

  GSList *extra_attrs;
};

/*< private>
 * PangoItem:
 * @offset: byte offset of the start of this item in text.
 * @length: length of this item in bytes.
 * @num_chars: number of Unicode characters in the item.
 * @char_offset: character offset of the start of this item in text. Since 1.50
 * @analysis: analysis results for the item.
 *
 * The `PangoItem` structure stores information about a segment of text.
 *
 * You typically obtain `PangoItems` by itemizing a piece of text
 * with [func@itemize].
 */
struct _PangoItem
{
  int offset;
  int length;
  int num_chars;
  int char_offset;
  PangoAnalysis analysis;
};


void               pango_analysis_collect_features    (const PangoAnalysis        *analysis,
                                                       hb_feature_t               *features,
                                                       guint                       length,
                                                       guint                      *num_features);

void               pango_analysis_set_size_font       (PangoAnalysis              *analysis,
                                                       PangoFont                  *font);
PangoFont *        pango_analysis_get_size_font       (const PangoAnalysis        *analysis);

GList *            pango_itemize_with_font            (PangoContext               *context,
                                                       PangoDirection              base_dir,
                                                       const char                 *text,
                                                       int                         start_index,
                                                       int                         length,
                                                       PangoAttrList              *attrs,
                                                       PangoAttrIterator          *cached_iter,
                                                       const PangoFontDescription *desc);

GList *            pango_itemize_post_process_items   (PangoContext               *context,
                                                       const char                 *text,
                                                       PangoLogAttr               *log_attrs,
                                                       GList                      *items);

void               pango_item_unsplit                 (PangoItem *orig,
                                                       int        split_index,
                                                       int        split_offset);


typedef struct _ItemProperties ItemProperties;
struct _ItemProperties
{
  guint uline_single        : 1;
  guint uline_double        : 1;
  guint uline_error         : 1;
  PangoUnderlinePosition uline_position;
  guint strikethrough       : 1;
  guint oline_single        : 1;
  guint showing_space       : 1;
  guint no_paragraph_break  : 1;
  int letter_spacing;
  int line_spacing;
  int absolute_line_height;
  double line_height;
};

void               pango_item_get_properties          (PangoItem        *item,
                                                       ItemProperties   *properties);
