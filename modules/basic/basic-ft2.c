/* Pango
 * basic-ft2.c:
 *
 * Copyright (C) 1999 Red Hat Software
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <glib.h>
#include <string.h>

#include "pango-layout.h"
#include "pango-engine.h"
#include "pangoft2.h"
#include "pango-utils.h"

#define SCRIPT_ENGINE_NAME "BasicScriptEngineFT2"


/* Zero Width characters:
 *
 *  200B  ZERO WIDTH SPACE
 *  200C  ZERO WIDTH NON-JOINER
 *  200D  ZERO WIDTH JOINER
 *  200E  LEFT-TO-RIGHT MARK
 *  200F  RIGHT-TO-LEFT MARK
 *  202A  LEFT-TO-RIGHT EMBEDDING
 *  202B  RIGHT-TO-LEFT EMBEDDING
 *  202C  POP DIRECTIONAL FORMATTING
 *  202D  LEFT-TO-RIGHT OVERRIDE
 *  202E  RIGHT-TO-LEFT OVERRIDE
 */

#define ZERO_WIDTH_CHAR(wc)\
(((wc) >= 0x200B && (wc) <= 0x200F) || ((wc) >= 0x202A && (wc) <= 0x202E))


static PangoEngineRange basic_ranges[] = {
  /* Basic Latin, Latin-1 Supplement, Latin Extended-A, Latin Extended-B,
   * IPA Extensions
   */
  { 0x0000, 0x02af, "*" }, 

  /* Spacing Modifier Letters */
  { 0x02b0, 0x02ff, "" },

  /* Not covered: Combining Diacritical Marks */

  /* Greek, Cyrillic, Armenian */
  { 0x0380, 0x058f, "*" },

  /* Hebrew */
  { 0x0591, 0x05f4, "" },

  /* Arabic */
  { 0x060c, 0x06f9, "" },

  /* Not covered: Syriac, Thaana, Devanagari, Bengali, Gurmukhi, Gujarati,
   * Oriya, Tamil, Telugu, Kannada, Malayalam, Sinhala
   */

  /* Thai */
  { 0x0e01, 0x0e5b, "" },  

  /* Not covered: Lao, Tibetan, Myanmar, Georgian, Hangul Jamo, Ethiopic,
   * Cherokee, Unified Canadian Aboriginal Syllabics, Ogham, Runic,
   * Khmer, Mongolian
   */

  /* Latin Extended Additional, Greek Extended */
  { 0x1e00, 0x1fff, "*" },

  /* General Punctuation, Superscripts and Subscripts, Currency Symbols,
   * Combining Marks for Symbols, Letterlike Symbols, Number Forms,
   * Arrows, Mathematical Operators, Miscellaneous Technical,
   * Control Pictures, Optical Character Recognition, Enclosed Alphanumerics,
   * Box Drawing, Block Elements, Geometric Shapes, Miscellaneous Symbols,
   * Dingbats, Braille Patterns, CJK Radicals Supplement, Kangxi Radicals,
   * Ideographic Description Characters, CJK Symbols and Punctuation,
   * Hiragana, Katakana, Bopomofo, Hangul Compatibility Jamo, Kanbun,
   * Bopomofo Extended, Enclosed CJK Letters and Months, CJK Compatibility,
   * CJK Unified Ideographs Extension A, CJK Unified Ideographs
   */
  { 0x2000, 0x9fff, "*" },

  /* Not covered: Yi Syllables, Yi Radicals */

  /* Hangul Syllables */
  { 0xac00, 0xd7a3, "kr" },

  /* Not covered: Private Use */

  /* CJK Compatibility Ideographs (partly) */
  { 0xf900, 0xfa0b, "kr" },

  /* Not covered: CJK Compatibility Ideographs (partly),
   * Alphabetic Presentation Forms, Arabic Presentation Forms-A,
   * Combining Half Marks, CJK Compatibility Forms,
   * Small Form Variants, Arabic Presentation Forms-B,
   * Specials
   */

  /* Halfwidth and Fullwidth Forms (partly) */
  { 0xff00, 0xffe3, "*" }

  /* Not covered: Halfwidth and Fullwidth Forms, Specials */
};

static PangoEngineInfo script_engines[] = {
  {
    SCRIPT_ENGINE_NAME,
    PANGO_ENGINE_TYPE_SHAPE,
    PANGO_RENDER_TYPE_FT2,
    basic_ranges, G_N_ELEMENTS(basic_ranges)
  }
};

static gint n_script_engines = G_N_ELEMENTS (script_engines);

/*
 * FT2 system script engine portion
 */

static PangoGlyph 
find_char (PangoFont *font,
	   gunichar   wc)
{
  FT_Face face;
  FT_UInt index;

  face = pango_ft2_font_get_face (font);
  index = FT_Get_Char_Index (face, wc);
  if (index && index <= face->num_glyphs)
    return index;

  return 0;
}

static void
set_glyph (PangoFont        *font,
	   PangoGlyphString *glyphs,
	   int               i,
	   int               offset,
	   PangoGlyph        glyph)
{
  PangoRectangle logical_rect;

  glyphs->glyphs[i].glyph = glyph;
  
  glyphs->glyphs[i].geometry.x_offset = 0;
  glyphs->glyphs[i].geometry.y_offset = 0;

  glyphs->log_clusters[i] = offset;

  pango_font_get_glyph_extents (font, glyphs->glyphs[i].glyph, NULL, &logical_rect);
  glyphs->glyphs[i].geometry.width = logical_rect.width;

  if (i > 0)
    {
      glyphs->glyphs[i-1].geometry.width +=
	pango_ft2_font_get_kerning (font,
				    glyphs->glyphs[i-1].glyph,
				    glyphs->glyphs[i].glyph);
    }
}

static void
swap_range (PangoGlyphString *glyphs,
	    int               start,
	    int               end)
{
  int i, j;
  
  for (i = start, j = end - 1; i < j; i++, j--)
    {
      PangoGlyphInfo glyph_info;
      gint log_cluster;
      
      glyph_info = glyphs->glyphs[i];
      glyphs->glyphs[i] = glyphs->glyphs[j];
      glyphs->glyphs[j] = glyph_info;
      
      log_cluster = glyphs->log_clusters[i];
      glyphs->log_clusters[i] = glyphs->log_clusters[j];
      glyphs->log_clusters[j] = log_cluster;
    }
}

static void 
basic_engine_shape (PangoFont        *font,
		    const char       *text,
		    gint              length,
		    PangoAnalysis    *analysis,
		    PangoGlyphString *glyphs)
{
  int n_chars;
  int i;
  const char *p;

  g_return_if_fail (font != NULL);
  g_return_if_fail (text != NULL);
  g_return_if_fail (length >= 0);
  g_return_if_fail (analysis != NULL);

  n_chars = g_utf8_strlen (text, length);
  pango_glyph_string_set_size (glyphs, n_chars);

  p = text;
  for (i = 0; i < n_chars; i++)
    {
      gunichar wc;
      gunichar mirrored_ch;
      PangoGlyph index;

      wc = g_utf8_get_char (p);

      if (analysis->level % 2)
	if (pango_get_mirror_char (wc, &mirrored_ch))
	  wc = mirrored_ch;

      if (wc == 0xa0)	/* non-break-space */
	wc = 0x20;
		
      if (ZERO_WIDTH_CHAR (wc))
	{
	  set_glyph (font, glyphs, i, p - text, 0);
	}
      else
	{
	  index = find_char (font, wc);
	  if (index)
	    {
	      set_glyph (font, glyphs, i, p - text, index);
	      
	      if (g_unichar_type (wc) == G_UNICODE_NON_SPACING_MARK)
		{
		  if (i > 0)
		    {
		      PangoRectangle logical_rect, ink_rect;
		      
		      glyphs->glyphs[i].geometry.width = MAX (glyphs->glyphs[i-1].geometry.width,
							      glyphs->glyphs[i].geometry.width);
		      glyphs->glyphs[i-1].geometry.width = 0;
		      glyphs->log_clusters[i] = glyphs->log_clusters[i-1];

		      /* Some heuristics to try to guess how overstrike glyphs are
		       * done and compensate
		       */
		      pango_font_get_glyph_extents (font, glyphs->glyphs[i].glyph, &ink_rect, &logical_rect);
		      if (logical_rect.width == 0 && ink_rect.x == 0)
			glyphs->glyphs[i].geometry.x_offset = (glyphs->glyphs[i].geometry.width - ink_rect.width) / 2;
		    }
		}
	    }
	  else
	    set_glyph (font, glyphs, i, p - text, pango_ft2_get_unknown_glyph (font));
	}
      
      p = g_utf8_next_char (p);
    }

  /* Simple bidi support... may have separate modules later */

  if (analysis->level % 2)
    {
      int start, end;

      /* Swap all glyphs */
      swap_range (glyphs, 0, n_chars);
      
      /* Now reorder glyphs within each cluster back to LTR */
      for (start = 0; start < n_chars;)
	{
	  end = start;
	  while (end < n_chars &&
		 glyphs->log_clusters[end] == glyphs->log_clusters[start])
	    end++;
	  
	  swap_range (glyphs, start, end);
	  start = end;
	}
    }
}

static PangoCoverage *
basic_engine_get_coverage (PangoFont  *font,
			   PangoLanguage *lang)
{
  return pango_font_get_coverage (font, lang);
}

static PangoEngine *
basic_engine_ft2_new (void)
{
  PangoEngineShape *result;
  
  result = g_new (PangoEngineShape, 1);

  result->engine.id = SCRIPT_ENGINE_NAME;
  result->engine.type = PANGO_ENGINE_TYPE_SHAPE;
  result->engine.length = sizeof (result);
  result->script_shape = basic_engine_shape;
  result->get_coverage = basic_engine_get_coverage;

  return (PangoEngine *)result;
}

/* The following three functions provide the public module API for
 * Pango
 */
#ifdef FT2_MODULE_PREFIX
#define MODULE_ENTRY(func) _pango_basic_ft2_##func
#else
#define MODULE_ENTRY(func) func
#endif

void 
MODULE_ENTRY(script_engine_list) (PangoEngineInfo **engines,
				  gint             *n_engines)
{
  *engines = script_engines;
  *n_engines = n_script_engines;
}

PangoEngine *
MODULE_ENTRY(script_engine_load) (const char *id)
{
  if (!strcmp (id, SCRIPT_ENGINE_NAME))
    return basic_engine_ft2_new ();
  else
    return NULL;
}

void 
MODULE_ENTRY(script_engine_unload) (PangoEngine *engine)
{
}

