#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/variable.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "ev3if.h"

typedef struct ev3_font_size {
  int32_t w;
  int32_t h;
} ev3_font_size;

static ev3_font_size _font_size[2] = {{0, 8}, {0, 16}};

/*
 *  call-seq:
 *     LCD.new(font, x, y, width, height, color)  # => LCD
 *
 *  Creates LCD object.
 *
 *  Parameters:
 *    +font+    font size.
 *       :small   small font (6x8, default)
 *       :medium  medium font (8x16)
 *    +x+       Window origin (left) X coordinate. (default: 0)
 *    +y+       Window origin (top) Y coordinate. (default: 0)
 *    +width+   Width of window. (default: LCD::WIDTH)
 *    +height+  Height of window. (default: LCD::HEIGHT)
 *    +color+   LCD foreground color.
 *       :black   black (default)
 *       :white   while
 *
 *  Returns LCD object.
 */
static mrb_value
mrb_lcd_init(mrb_state *mrb, mrb_value self)
{
  struct RClass *lcd = mrb_obj_class(mrb, self);
  mrb_value fmap = mrb_const_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "FONT"));
  mrb_value cmap = mrb_const_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "COLOR"));
  mrb_sym font = mrb_intern_lit(mrb, "small");
  mrb_sym col = mrb_intern_lit(mrb, "black");
  mrb_value fontv;
  mrb_value colv;
  mrb_int fonti;
  mrb_int x = 0;
  mrb_int y = 0;
  mrb_int w = EV3_LCD_WIDTH;
  mrb_int h = EV3_LCD_HEIGHT;

  mrb_get_args(mrb, "|niiiin", &font, &x, &y, &w, &h, &col);

  fontv = mrb_hash_get(mrb, fmap, mrb_symbol_value(font));
  if (mrb_nil_p(fontv)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid font size :%S", mrb_sym2str(mrb, font));
  }
  fonti = mrb_fixnum(fontv);
  if (_font_size[fonti].w == 0) {
    /* initialize font size at 1st time */
    ev3_font_get_size(fonti, &_font_size[fonti].w, &_font_size[fonti].h);
  }

  colv = mrb_hash_get(mrb, cmap, mrb_symbol_value(col));
  if (mrb_nil_p(colv)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid foreground color :%S", mrb_sym2str(mrb, col));
  }

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@font"),   fontv);
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@left"),   mrb_fixnum_value(x));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@top"),    mrb_fixnum_value(y));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@width"),  mrb_fixnum_value(w));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@height"), mrb_fixnum_value(h));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@color"),  colv);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cx"),     mrb_fixnum_value(0));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cy"),     mrb_fixnum_value(0));

  return self;
}

/*
 *  call-seq:
 *     LCD.font = font  # => nil
 *
 *  Set the current font.
 *
 *  Parameters:
 *    +font+    font size.
 *       :small     small font
 *       :medium    medium font (8x16)
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_set_font(mrb_state *mrb, mrb_value self)
{
  mrb_value fmap = mrb_const_get(mrb, self, mrb_intern_lit(mrb, "FONT"));
  mrb_sym font;
  mrb_value fontv;

  mrb_get_args(mrb, "n", &font);
  fontv = mrb_hash_get(mrb, fmap, mrb_symbol_value(font));
  if (mrb_nil_p(fontv)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid font size :%S", mrb_sym2str(mrb, font));
  }

  ev3_lcd_set_font(mrb_fixnum(fontv));
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@@font"), fontv);

  return mrb_nil_value();
}

static void
mrb_lcd_get_font_size(mrb_state *mrb, mrb_value lcd, mrb_int *fw, mrb_int *fh)
{
  mrb_value font = mrb_cv_get(mrb, lcd, mrb_intern_lit(mrb, "@@font"));
  ev3_font_get_size(mrb_fixnum(font), fw, fh);
}

static void
ev3_set_current_font(mrb_state *mrb, mrb_value self)
{
  static mrb_bool first = TRUE;
  struct RClass *lcd = mrb_obj_class(mrb, self);
  mrb_int current = mrb_fixnum(mrb_cv_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "@@font")));
  mrb_int target  = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@font")));

  if (first || current != target) {
    ev3_lcd_set_font((uint16_t)target);
    mrb_cv_set(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "@@font"), mrb_fixnum_value(target));
    first = FALSE;
  }
}

static void
mrb_lcd_lf(mrb_state *mrb, mrb_value lcd)
{
  mrb_int sh = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@height")));
#ifdef EV3
  mrb_int sw = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@width")));
  mrb_int x0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@left")));
  mrb_int y0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@top")));
  mrb_int fc = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@color")));
  mrb_int cx = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cx")));
#endif
  mrb_int cy = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cy")));
  mrb_int font = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@font")));
  mrb_assert(font == EV3_FONT_SMALL || font == EV3_FONT_MEDIUM);
  ev3_font_size *f = &_font_size[font];
  mrb_assert(f->w != 0 && f->h != 0);
  mrb_int ch = sh / f->h;

#ifdef EV3
  ev3_lcd_fill_rect(x0+cx*f->w, y0+cy*f->h, sw-cx*f->w, f->h, fc ? 0 : 1);
#else
  MRBEV3_PUTS("");
#endif

  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(0));
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value((cy+1) % ch));
}

static void
mrb_lcd_print_line(mrb_state *mrb, mrb_value lcd, mrb_value *str)
{
  const char *src = mrb_string_value_cstr(mrb, str);
  size_t len = strlen(src);
  char *buf = mrb_malloc(mrb, len+1);
  mrb_int font = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@font")));
  mrb_assert(font == EV3_FONT_SMALL || font == EV3_FONT_MEDIUM);

  mrb_assert(_font_size[font].w != 0 && _font_size[font].h != 0);
  mrb_int x0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@left")));
  mrb_int y0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@top")));
  mrb_int sw = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@width")));
  mrb_int sh = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@height")));
#ifdef EV3
  mrb_int fc = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@color")));
#endif
  mrb_int cx = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cx")));
  mrb_int cy = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cy")));
  mrb_int cw = sw / _font_size[font].w;
  mrb_int ch = sh / _font_size[font].h;
  ev3_font_size *f = &_font_size[font];
  mrb_assert(f->w != 0 && f->h != 0);
  mrb_int csr = cx;
  char *dst;
  mrb_bool lf;

  memset(buf, 0, len+1);
  dst = buf;
  while (len--) {
    lf = FALSE;
    if (*src == '\n') {
      lf = TRUE;
      src++;
    }
    else {
      *dst++ = *src++;
      csr++;
      if (csr >= cw) {
        lf = TRUE;
      }
    }
    if (lf) {
      *dst = '\0';
      ev3_lcd_draw_string(buf, x0+cx*f->w, y0+cy*f->h);

      /* line feed */
#ifdef EV3
      ev3_lcd_fill_rect(x0+csr*f->w, y0+cy*f->h, sw-csr*f->w, f->h, fc ? 0 : 1);
#else
      MRBEV3_PUTS("");
#endif
      cy = (cy+1) % ch;
      cx = csr = 0;

      memset(buf, 0, strlen(src)+1);
      dst = buf;
    }
  }
  *dst = '\0';
  if (dst != buf) {
    ev3_lcd_draw_string(buf, x0+cx*f->w, y0+cy*f->h);
  }

  mrb_free(mrb, buf);

  /* update cursor */
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(csr));
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(cy));
}

/*
 *  call-seq:
 *     lcd.print(...)  # => nil
 *
 *  Prints the string at cursor position on window.
 *
 *  Parameters:
 *    +...+   The target for print.
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_print(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *args;
  mrb_int i;
  mrb_value s;

  mrb_get_args(mrb, "*", &args, &argc);

  ev3_set_current_font(mrb, self);

  for(i=0; i<argc; i++,args++) {
    s = mrb_funcall(mrb, *args, "to_s", 0);
    mrb_lcd_print_line(mrb, self, &s);
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     lcd.puts(...)  # => nil
 *
 *  Puts the string that added newline at current cursor position
 *  on window.
 *
 *  Parameters:
 *    +...+   The target for put.
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_puts(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *args;
  mrb_int i;
  mrb_value s;

  mrb_get_args(mrb, "*", &args, &argc);

  ev3_set_current_font(mrb, self);

  if (argc == 0) {
    mrb_lcd_lf(mrb, self);
  }
  for(i=0; i<argc; i++,args++) {
    s = mrb_funcall(mrb, *args, "to_s", 0);
    mrb_lcd_print_line(mrb, self, &s);
    mrb_lcd_lf(mrb, self);
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LCD.draw(x, y, str)  # => nil
 *
 *  Draw the string at specified coordinate of LCD.
 *
 *  Parameters:
 *    +x+     X-coordinate of the string left edge
 *    +y+     Y-coordinate of the string top edge
 *    +str+   The target for drawing.
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_draw_string(mrb_state *mrb, mrb_value self)
{
  mrb_value obj, str;
  mrb_int x, y;
  mrb_int fw, fh;
  mrb_int cw, ch;

  mrb_lcd_get_font_size(mrb, self, &fw, &fh);
  cw = EV3_LCD_WIDTH  / fw;
  ch = EV3_LCD_HEIGHT / fh;

  mrb_get_args(mrb, "iio", &x, &y, &obj);

  if (mrb_string_p(obj)) {
    str = obj;
  }
  else {
    str = mrb_funcall(mrb, obj, "to_s", 0);
  }

  ev3_lcd_draw_string(mrb_string_value_cstr(mrb, &str), x*cw, y*ch);

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     lcd.locate(cx, cy)  # => nil
 *
 *  Set the cursor position on window.
 *
 *  Parameters:
 *    +cx+    X-coordinate of cursor
 *    +cy+    Y-coordinate of cursor
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_locate(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y = -1;

  mrb_get_args(mrb, "i|i", &x, &y);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(x));
  if (y >= 0) {
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(y));
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     lcd.clear(col)  # => nil
 *
 *  Clear window.
 *
 *  Parameters:
 *    +col+     LCD foreground color.
 *       :black   black (default)
 *       :white   while
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_clear(mrb_state *mrb, mrb_value self)
{
  struct RClass *lcd = mrb_obj_class(mrb, self);
  mrb_value cmap = mrb_const_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "COLOR"));
  mrb_sym col = mrb_intern_lit(mrb, "black");
  mrb_value colv;
  mrb_int x, y, w, h;

  mrb_get_args(mrb, "|n", &col);

  colv = mrb_hash_get(mrb, cmap, mrb_symbol_value(col));
  if (mrb_nil_p(colv)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid foreground color :%S", mrb_sym2str(mrb, col));
  }

  x = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left")));
  y = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@top")));
  w = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@width")));
  h = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@height")));
  ev3_lcd_fill_rect(x, y, w, h, mrb_fixnum(colv) ? 0 : 1);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(0));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(0));

  return mrb_nil_value();
}

void
mrb_ev3_lcd_init(mrb_state *mrb, struct RClass *ev3)
{
  struct RClass *lcd;
  mrb_value lcdo;
  mrb_value font;
  mrb_value col;

  /* LCD class */
  lcd = mrb_define_class_under(mrb, ev3, "LCD", mrb->object_class);
  lcdo = mrb_obj_value(lcd);

  font = mrb_hash_new(mrb);
  mrb_hash_set(mrb, font, mrb_symbol_value(mrb_intern_lit(mrb, "small")),   mrb_fixnum_value(EV3_FONT_SMALL));
  mrb_hash_set(mrb, font, mrb_symbol_value(mrb_intern_lit(mrb, "medium")),  mrb_fixnum_value(EV3_FONT_MEDIUM));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "FONT"), font);

  col = mrb_hash_new(mrb);
  mrb_hash_set(mrb, col, mrb_symbol_value(mrb_intern_lit(mrb, "white")), mrb_fixnum_value(EV3_LCD_WHITE));
  mrb_hash_set(mrb, col, mrb_symbol_value(mrb_intern_lit(mrb, "black")), mrb_fixnum_value(EV3_LCD_BLACK));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "COLOR"), col);

  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "WIDTH"),        mrb_fixnum_value(EV3_LCD_WIDTH));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "HEIGHT"),       mrb_fixnum_value(EV3_LCD_HEIGHT));

  mrb_mod_cv_set(mrb, lcd, mrb_intern_lit(mrb, "@@font"),       mrb_fixnum_value(EV3_FONT_SMALL));

  mrb_define_class_method(mrb, lcd, "font=", mrb_lcd_set_font,    MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, lcd, "draw",  mrb_lcd_draw_string, MRB_ARGS_REQ(3));

  mrb_define_method(mrb, lcd, "initialize", mrb_lcd_init,   MRB_ARGS_OPT(6));
  mrb_define_method(mrb, lcd, "print",      mrb_lcd_print,  MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "puts",       mrb_lcd_puts,   MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "locate",     mrb_lcd_locate, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, lcd, "clear",      mrb_lcd_clear,  MRB_ARGS_OPT(1));

  // mrb_undef_method(mrb, mrb->kernel_module, "__printstr__");
  // mrb_define_method(mrb, mrb->kernel_module, "__printstr__", mrb_lcd_print, MRB_ARGS_ANY());
}
