#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/hash.h"
#include "ev3if.h"

typedef struct ev3_font_size {
  int32_t w;
  int32_t h;
} ev3_font_size;

typedef struct tone_info {
  const char *tone;
  mrb_int freq;
} tone_info;

static ev3_font_size _font_size[2] = {{0, 8}, {0, 16}};

#define ROUND(x) ((mrb_int)(x + 0.5))

static const tone_info _tones[] =
{
  {"c4",  ROUND( 261.63)}, {"cs4", ROUND( 277.18)}, {"d4",  ROUND( 293.66)},
  {"ds4", ROUND( 311.13)}, {"e4",  ROUND( 329.63)}, {"f4",  ROUND( 349.23)},
  {"fs4", ROUND( 369.99)}, {"g4",  ROUND( 392.00)}, {"gs4", ROUND( 415.30)},
  {"a4",  ROUND( 440.00)}, {"as4", ROUND( 466.16)}, {"b4",  ROUND( 493.88)},
  {"c5",  ROUND( 523.25)}, {"cs5", ROUND( 554.37)}, {"d5",  ROUND( 587.33)},
  {"ds5", ROUND( 622.25)}, {"e5",  ROUND( 659.25)}, {"f5",  ROUND( 698.46)},
  {"fs5", ROUND( 739.99)}, {"g5",  ROUND( 783.99)}, {"gs5", ROUND( 830.61)},
  {"a5",  ROUND( 880.00)}, {"as5", ROUND( 932.33)}, {"b5",  ROUND( 987.77)},
  {"c6",  ROUND(1046.50)}, {"cs6", ROUND(1108.73)}, {"d6",  ROUND(1174.66)},
  {"ds6", ROUND(1244.51)}, {"e6",  ROUND(1318.51)}, {"f6",  ROUND(1396.91)},
  {"fs6", ROUND(1479.98)}, {"g6",  ROUND(1567.98)}, {"gs6", ROUND(1661.22)},
  {"a6",  ROUND(1760.00)}, {"as6", ROUND(1864.66)}, {"b6",  ROUND(1975.53)},
  {"c6",  ROUND(1046.50)}, {"cs6", ROUND(1108.73)}, {"d6",  ROUND(1174.66)},
  {"ds6", ROUND(1244.51)}, {"e6",  ROUND(1318.51)}, {"f6",  ROUND(1396.91)},
  {"fs6", ROUND(1479.98)}, {"g6",  ROUND(1567.98)}, {"gs6", ROUND(1661.22)},
  {"a6",  ROUND(1760.00)}, {"as6", ROUND(1864.66)}, {"b6",  ROUND(1975.53)}
};


/*
 *  call-seq:
 *     delay(ms)  # => nil
 *
 *  Delay the processing of a task for the specified number of milliseconds.
 *
 *  Returns nil.
 */
static mrb_value
rtos_delay(mrb_state *mrb, mrb_value self)
{
  mrb_int t;
  mrb_get_args(mrb, "i", &t);
  EV3_delay(t);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LCD.new(font, x, y, width, height, color)  # => LCD
 *
 *  Creates LCD object.
 *
 *  Parameters:
 *    +font+    font size.
 *       LCD::SMALL_FONT:  small font (6x8, default)
 *       LCD::MEDIUM_FONT: medium font (8x16)
 *    +x+       Window origin (left) X coordinate. (default: 0)
 *    +y+       Window origin (top) Y coordinate. (default: 0)
 *    +width+   Width of window. (default: LCD::WIDTH)
 *    +height+  Height of window. (default: LCD::HEIGHT)
 *    +color+   LCD foreground color.
 *       LCD::BLACK:  black (default)
 *       LCD::WHITE:  while
 *
 *  Returns LCD object.
 */
static mrb_value
mrb_lcd_init(mrb_state *mrb, mrb_value self)
{
  mrb_int font = EV3_FONT_SMALL;
  mrb_int x = 0;
  mrb_int y = 0;
  mrb_int w = EV3_LCD_WIDTH;
  mrb_int h = EV3_LCD_HEIGHT;
  mrb_int col = EV3_LCD_BLACK;

  mrb_get_args(mrb, "|iiiiii", &font, &x, &y, &w, &h, &col);

  if (_font_size[font].w == 0) {
    /* initialize font size at 1st time */
    EV3_font_get_size(font, &_font_size[font].w, &_font_size[font].h);
  }

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@font"),   mrb_fixnum_value(font));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@left"),   mrb_fixnum_value(x));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@top"),    mrb_fixnum_value(y));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@width"),  mrb_fixnum_value(w));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@height"), mrb_fixnum_value(h));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@color"),  mrb_fixnum_value(col));

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
 *       LCD::SMALL_FONT:  small font
 *       LCD::MEDIUM_FONT: medium font (8x16)
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_set_font(mrb_state *mrb, mrb_value self)
{
  mrb_int font;

  mrb_get_args(mrb, "i", &font);

  EV3_lcd_set_font((int16_t)font);
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@@font"), mrb_fixnum_value(font));

  return mrb_nil_value();
}

static void
mrb_lcd_get_font_size(mrb_state *mrb, mrb_value lcd, mrb_int *fw, mrb_int *fh)
{
  mrb_value font = mrb_cv_get(mrb, lcd, mrb_intern_lit(mrb, "@@font"));
  EV3_font_get_size(mrb_fixnum(font), fw, fh);
}

static void
ev3_set_current_font(mrb_state *mrb, mrb_value self)
{
  static mrb_bool first = TRUE;
  struct RClass *lcd = mrb_obj_class(mrb, self);
  mrb_int current = mrb_fixnum(mrb_cv_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "@@font")));
  mrb_int target  = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@font")));

  if (first || current != target) {
    EV3_lcd_set_font((uint16_t)target);
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

  EV3_lcd_fill_rect(x0+cx*f->w, y0+cy*f->h, sw-cx*f->w, f->h, fc ? 0 : 1);
#else
  puts("");
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
      EV3_lcd_draw_string(buf, x0+cx*f->w, y0+cy*f->h);

      /* line feed */
#ifdef EV3
      EV3_lcd_fill_rect(x0+cx*f->w, y0+cy*f->h, sw-x0+cx*f->w, f->h, fc ? 0 : 1);
#else
      puts("");
#endif
      cy = (cy+1) % ch;
      cx = csr = 0;

      memset(buf, 0, strlen(src)+1);
      dst = buf;
    }
  }
  *dst = '\0';
  if (dst != buf) {
    EV3_lcd_draw_string(buf, x0+cx*f->w, y0+cy*f->h);
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

  EV3_lcd_draw_string(mrb_string_value_cstr(mrb, &str), x*cw, y*ch);

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     lcd.cx  # => Fixnum
 *
 *  Get the X-coordinate of cursor on window.
 *
 *  Returns X-coordinate.
 */
static mrb_value
mrb_lcd_cx(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cx"));
}

/*
 *  call-seq:
 *     lcd.cy  # => Fixnum
 *
 *  Get the Y-coordinate of cursor on window.
 *
 *  Returns Y-coordinate.
 */
static mrb_value
mrb_lcd_cy(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cy"));
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
 *     lcd.clear  # => nil
 *
 *  Clear window.
 *
 *  Returns nil.
 */
static mrb_value
mrb_lcd_clear(mrb_state *mrb, mrb_value self)
{
  mrb_int col = -1;
  mrb_int x, y, w, h;

  mrb_get_args(mrb, "|i", &col);

  if (col < 0) {
    struct RClass *lcd = mrb_obj_class(mrb, self);
    col = mrb_fixnum(mrb_cv_get(mrb, mrb_obj_value(lcd), mrb_intern_lit(mrb, "@@font"))) ? 0 : 1;
  }

  x = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@left")));
  y = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@top")));
  w = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@width")));
  h = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@height")));
  EV3_lcd_fill_rect(x, y, w, h, col);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(0));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(0));

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     Button.new(key)  # => Button
 *
 *  Creates Button object.
 *
 *  Parameters:
 *    +key+     Button name
 *       :left    Left button
 *       :right   Right button
 *       :up      Up button
 *       :down    Down button
 *       :enter   Enter button
 *       :back    Back button
 *
 *  Returns Butotn object.
 */
static mrb_value
mrb_btn_init(mrb_state *mrb, mrb_value self)
{
  struct RClass *btn = mrb_obj_class(mrb, self);
  mrb_value map = mrb_cv_get(mrb, mrb_obj_value(btn), mrb_intern_lit(mrb, "@@map"));
  mrb_sym key;
  mrb_value keyv;

  mrb_get_args(mrb, "n", &key);
  keyv = mrb_hash_get(mrb, map, mrb_symbol_value(key));

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@key"), keyv);

  return self;
}

/*
 *  call-seq:
 *     Button[key]  # => Button
 *
 *  Get button object.
 *
 *  Parameters:
 *    +key+     Button name
 *       :left    Left button
 *       :right   Right button
 *       :up      Up button
 *       :down    Down button
 *       :enter   Enter button
 *       :back    Back button
 *
 *  Returns Button object
 */
static mrb_value
mrb_btn_get(mrb_state *mrb, mrb_value self)
{
  mrb_sym sym;
  mrb_value symv;

  mrb_get_args(mrb, "n", &sym);
  symv = mrb_symbol_value(sym);

  return mrb_obj_new(mrb, mrb_class_ptr(self), 1, &symv);
}

/*
 *  call-seq:
 *     btn.pressed?  # => true/false
 *
 *  Get button status.
 *
 *  Returns button status. (true:pressed, false:released)
 */
static mrb_value
mrb_btn_pressed(mrb_state *mrb, mrb_value self)
{
  int32_t pressed = FALSE;
  mrb_value key;

  key = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@key"));
  if (!mrb_nil_p(key)) {
    pressed = EV3_button_is_pressed((int16_t)mrb_fixnum(key));
  }

  return mrb_bool_value(pressed);
}

/*
 *  call-seq:
 *     LED.color = col  # => nil
 *
 *  Light LED.
 *
 *  Parameters:
 *    +col+     LED color
 *       LED::OFF
 *       LED::RED
 *       LED::GREEN
 *       LED::ORANGE
 *
 *  Returns nil.
 */
static mrb_value
mrb_led_color(mrb_state *mrb, mrb_value self)
{
  mrb_int col;
  mrb_get_args(mrb, "i", &col);
  EV3_led_set_color(col);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LED.off  # => nil
 *
 *  LED turn off.
 *
 *  Returns nil.
 */
static mrb_value
mrb_led_off(mrb_state *mrb, mrb_value self)
{
  EV3_led_set_color(LED_OFF);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     Motot.new(port, type)  # => Motor
 *
 *  Creates Motor object.
 *
 *  Parameters:
 *    +port+    Motor port
 *       Motor::PORT_A
 *       Motor::PORT_B
 *       Motor::PORT_C
 *       Motor::PORT_D
 *    +type+    Motor type
 *       Motor::NONE      none
 *       Motor::MEDIUM    medium motor
 *       Motor::LARGE     large motor
 *       Motor::UNKNOWN   Unregulated motor
 *
 *  Returns Motor object.
 */
static mrb_value
mrb_motor_init(mrb_state *mrb, mrb_value self)
{
  mrb_int port, type;

  mrb_get_args(mrb, "ii", &port, &type);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@port"), mrb_fixnum_value(port));
  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@type"), mrb_fixnum_value(type));

  return self;
}

/*
 *  call-seq:
 *     morot.power = pwr  # => Fixnum
 *
 *  Sets motor power.
 *
 *  Parameters:
 *    +pwr+     Motor power (-100 to 100)
 *         > 0:   Motor forwards
 *         < 0:   Motor backwards
 *
 *  Returns power value.
 */
static mrb_value
mrb_motor_set_power(mrb_state *mrb, mrb_value self)
{
  mrb_int port;
  mrb_int v;

  mrb_get_args(mrb, "i", &v);
  if      (v > EV3_MOTOR_FMAX) v = EV3_MOTOR_FMAX;
  else if (v < EV3_MOTOR_BMAX) v = EV3_MOTOR_BMAX;

  port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  EV3_motor_set_power(port, v);

  return mrb_fixnum_value(v);
}

/*
 *  call-seq:
 *     morot.power  # => Fixnum
 *
 *  Gets motor current power.
 *
 *  Returns power value.
 */
static mrb_value
mrb_motor_get_power(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_motor_get_power(port));
}

/*
 *  call-seq:
 *     morot.stop(brk)  # => nil
 *
 *  Stop the motor.
 *
 *  Parameters:
 *    +brk+
 *         true:  Brake motor
 *         false: Turn off motor
 *
 *  Returns nil
 */
static mrb_value
mrb_motor_stop(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  mrb_bool brake = FALSE;

  mrb_get_args(mrb, "|b", &brake);
  EV3_motor_stop(port, brake ? TRUE : FALSE);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     morot.rotate(deg, spd, blk)  # => nil
 *
 *  Rotate the motor.
 *
 *  Parameters:
 *    +deg+   Rotates through `deg` degrees
 *      > 0:    Rotate forward
 *      < 0:    Rotate backward
 *    +spd+   Rotate speed. (0 to 100)
 *    +blk+   Blocking mode
 *      false:  No blocking (default)
 *      true:   Blocking until completion
 *
 *  Returns nil
 */
static mrb_value
mrb_motor_rotate(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  mrb_int deg, spd, blk = 1;

  mrb_get_args(mrb, "ii|i", &deg, &spd, &blk);
  spd = (spd < 0) ? 0 : (spd > 100) ? 100 : spd;
  EV3_motor_rotate(port, deg, spd, blk);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     morot.count  # => Fixnum
 *
 *  Gets motor counter.
 *
 *  Returns counter value.
 */
static mrb_value
mrb_motor_get_count(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  return mrb_fixnum_value(EV3_motor_get_count(port));
}

/*
 *  call-seq:
 *     morot.reset  # => nil
 *
 *  Resets motor counter.
 *
 *  Returns counter value.
 */
static mrb_value
mrb_motor_reset_count(mrb_state *mrb, mrb_value self)
{
  mrb_int port = mrb_fixnum(mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@port")));
  EV3_motor_reset_count(port);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     morot.type  # => Fixnum
 *
 *  Gets motor type.
 *
 *  Returns counter type.
 *       Motor::NONE      none
 *       Motor::MEDIUM    medium motor
 *       Motor::LARGE     large motor
 *       Motor::UNKNOWN   Unregulated motor
 */
static mrb_value
mrb_motor_get_type(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@type"));
}

/*
 *  call-seq:
 *     Battery.mA  # => Fixnum
 *
 *  Get the current draw from the battery
 *
 *  Returns current in mA
 */
static mrb_value
mrb_battery_mA(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(EV3_battery_current_mA());
}

/*
 *  call-seq:
 *     Battery.mV  # => Fixnum
 *
 *  Get battery battery voltage.
 *
 *  Returns battery voltage in mV
 */
static mrb_value
mrb_battery_mV(mrb_state *mrb, mrb_value self)
{
  return mrb_fixnum_value(EV3_battery_voltage_mV());
}

/*
 *  call-seq:
 *     Sound.tone(t, ms)  # => nil
 *
 *  Play a tone, given its tone and duration.
 *    tone: <k>[s]<o>   (e.g. cs4)
 *      k: Key of tone
 *        c   DO
 *        d   RE
 *        e   MI
 *        f   FA
 *        g   SOL
 *        a   LA
 *        b   SI
 *      s: Sharp
 *      o: Octave (4 to 6)
 *
 *  Parameters:
 *    +t+     Tone of sound
 *        Symbol: Symbol of tone (:c4, :cs4, :d4, ds4, ... :b5)
 *        String: String of tone ("c4", "cs4", "d4", "ds4", ... "b5")
 *        Float:  Frequency of tone (Hz)
 *    +ms+    Duration of the tone. (milliseconds)
 *
 *  Returns nil
 */
static mrb_value
mrb_sound_tone(mrb_state *mrb, mrb_value self)
{
  mrb_value obj, map, tone = mrb_nil_value();
  mrb_int ms;
  mrb_sym sym;

  mrb_get_args(mrb, "oi", &obj, &ms);
  if (mrb_fixnum_p(obj)) {
    tone = obj;
  }
  else if (mrb_float_p(obj)) {
    tone = mrb_fixnum_value((mrb_int)(mrb_float(obj) + 0.5));
  }
  else {
    map = mrb_cv_get(mrb, self, mrb_intern_lit(mrb, "@@tones"));
    if (mrb_symbol_p(obj)) {
      tone = mrb_hash_get(mrb, map, obj);
    }
    else if (mrb_string_p(obj)) {
      sym = mrb_intern_cstr(mrb, mrb_string_value_cstr(mrb, &obj));
      tone = mrb_hash_get(mrb, map, mrb_symbol_value(sym));
    }
  }
  if (mrb_nil_p(tone)) {
    mrb_raisef(mrb, E_ARGUMENT_ERROR, "invalid tone (%S)", obj);
  }

  EV3_speaker_play_tone(mrb_fixnum(tone), ms);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     Sound.volume = vol  # => nil
 *
 *  Set volume level.
 *
 *  Parameters:
 *    +vol+   Volume level (0 to 100)
 *
 *  Returns nil
 */
static mrb_value
mrb_sound_set_volume(mrb_state *mrb, mrb_value self)
{
  mrb_int vol;
  mrb_get_args(mrb, "i", &vol);
  EV3_speaker_set_volume((uint16_t)vol);
  return mrb_nil_value();
}

/*
 *  call-seq:
 *     Sound.stop  # => nil
 *
 *  Stop sound.
 *
 *  Returns nil
 */
static mrb_value
mrb_sound_stop(mrb_state *mrb, mrb_value self)
{
  EV3_speaker_stop();
  return mrb_nil_value();
}

void
mrb_mruby_ev3_gem_init(mrb_state *mrb)
{
  struct RClass *ev3;
  struct RClass *lcd;
  struct RClass *btn;
  struct RClass *led;
  struct RClass *mtr;
  struct RClass *bat;
  struct RClass *snd;
  mrb_value lcdo;
  mrb_value keys;
  mrb_value ledo;
  mrb_value mtro;
  mrb_value tones;
  int i;

  /* EV3 module */
  ev3 = mrb_define_module(mrb, "EV3");

  /* RTOS api */
  mrb_define_method(mrb, ev3, "delay", rtos_delay, MRB_ARGS_REQ(1));

  /* LCD class */
  lcd = mrb_define_class_under(mrb, ev3, "LCD", mrb->object_class);
  lcdo = mrb_obj_value(lcd);

  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "WIDTH"),        mrb_fixnum_value(EV3_LCD_WIDTH));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "HEIGHT"),       mrb_fixnum_value(EV3_LCD_HEIGHT));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "SMALL_FONT"),   mrb_fixnum_value(EV3_FONT_SMALL));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "MEDIUM_FONT"),  mrb_fixnum_value(EV3_FONT_MEDIUM));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "WHITE"),        mrb_fixnum_value(EV3_LCD_WHITE));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "BLACK"),        mrb_fixnum_value(EV3_LCD_BLACK));

  mrb_mod_cv_set(mrb, lcd, mrb_intern_lit(mrb, "@@font"),       mrb_fixnum_value(EV3_FONT_SMALL));

  mrb_define_class_method(mrb, lcd, "font=", mrb_lcd_set_font,        MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, lcd, "draw",  mrb_lcd_draw_string, MRB_ARGS_REQ(3));

  mrb_define_method(mrb, lcd, "initialize", mrb_lcd_init,   MRB_ARGS_OPT(6));
  mrb_define_method(mrb, lcd, "print",      mrb_lcd_print,  MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "puts",       mrb_lcd_puts,   MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "cx",         mrb_lcd_cx,     MRB_ARGS_NONE());
  mrb_define_method(mrb, lcd, "cy",         mrb_lcd_cy,     MRB_ARGS_NONE());
  mrb_define_method(mrb, lcd, "locate",     mrb_lcd_locate, MRB_ARGS_ARG(1, 1));
  mrb_define_method(mrb, lcd, "clear",      mrb_lcd_clear,  MRB_ARGS_OPT(1));

  /* Button class */
  btn = mrb_define_class_under(mrb, ev3, "Button", mrb->object_class);

  keys = mrb_hash_new(mrb);
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "left")), mrb_fixnum_value(LEFT_BUTTON));
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "right")), mrb_fixnum_value(RIGHT_BUTTON));
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "up")), mrb_fixnum_value(UP_BUTTON));
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "down")), mrb_fixnum_value(DOWN_BUTTON));
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "enter")), mrb_fixnum_value(ENTER_BUTTON));
  mrb_hash_set(mrb, keys, mrb_symbol_value(mrb_intern_lit(mrb, "back")), mrb_fixnum_value(BACK_BUTTON));
  mrb_mod_cv_set(mrb, btn, mrb_intern_lit(mrb, "@@map"), keys);

  mrb_define_class_method(mrb, btn, "[]", mrb_btn_get, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, btn, "initialize", mrb_btn_init,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, btn, "pressed?",   mrb_btn_pressed,  MRB_ARGS_NONE());

  /* LED class */
  led = mrb_define_class_under(mrb, ev3, "LED", mrb->object_class);
  ledo = mrb_obj_value(led);

  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "OFF"),    mrb_fixnum_value(LED_OFF));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "RED"),    mrb_fixnum_value(LED_RED));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "GREEN"),  mrb_fixnum_value(LED_GREEN));
  mrb_const_set(mrb, ledo, mrb_intern_lit(mrb, "ORANGE"), mrb_fixnum_value(LED_ORANGE));

  mrb_define_class_method(mrb, led, "color=", mrb_led_color,  MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, led, "off",    mrb_led_off,    MRB_ARGS_NONE());

  /* Motor class */
  mtr = mrb_define_class_under(mrb, ev3, "Motor", mrb->object_class);
  mtro = mrb_obj_value(mtr);

  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_A"),   mrb_fixnum_value(EV3_PORT_A));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_B"),   mrb_fixnum_value(EV3_PORT_B));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_C"),   mrb_fixnum_value(EV3_PORT_C));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "PORT_D"),   mrb_fixnum_value(EV3_PORT_D));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "NONE"),     mrb_fixnum_value(NONE_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "MEDIUM"),   mrb_fixnum_value(MEDIUM_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "LARGE"),    mrb_fixnum_value(LARGE_MOTOR));
  mrb_const_set(mrb, mtro, mrb_intern_lit(mrb, "UNKNOWN"),  mrb_fixnum_value(UNREGULATED_MOTOR));

  mrb_define_method(mrb, mtr, "initialize",   mrb_motor_init,         MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mtr, "power=",       mrb_motor_set_power,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mtr, "power",        mrb_motor_get_power,    MRB_ARGS_NONE());
  mrb_define_method(mrb, mtr, "stop",         mrb_motor_stop,         MRB_ARGS_OPT(1));
  mrb_define_method(mrb, mtr, "rotate",       mrb_motor_rotate,       MRB_ARGS_ARG(2, 1));
  mrb_define_method(mrb, mtr, "count",        mrb_motor_get_count,    MRB_ARGS_NONE());
  mrb_define_method(mrb, mtr, "reset_count",  mrb_motor_reset_count,  MRB_ARGS_NONE());
  mrb_define_method(mrb, mtr, "type",         mrb_motor_get_type,     MRB_ARGS_NONE());

  /* Battery class */
  bat = mrb_define_class_under(mrb, ev3, "Battery", mrb->object_class);

  mrb_define_class_method(mrb, bat, "mA", mrb_battery_mA, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, bat, "mV", mrb_battery_mV, MRB_ARGS_NONE());

  /* Sound class */
  snd = mrb_define_class_under(mrb, ev3, "Sound", mrb->object_class);

  tones = mrb_hash_new(mrb);
  for (i=0; i<sizeof(_tones)/sizeof(_tones[0]); i++) {
    mrb_hash_set(mrb, tones, mrb_symbol_value(mrb_intern_cstr(mrb, _tones[i].tone)), mrb_fixnum_value(_tones[i].freq));
  }
  mrb_mod_cv_set(mrb, snd, mrb_intern_lit(mrb, "@@tones"), tones);

  mrb_define_class_method(mrb, snd, "tone",     mrb_sound_tone,       MRB_ARGS_REQ(2));
  mrb_define_class_method(mrb, snd, "volume=",  mrb_sound_set_volume, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, snd, "stop",     mrb_sound_stop,       MRB_ARGS_NONE());

  /* Object includes EV3 */
  mrb_include_module(mrb, mrb->object_class, ev3);
}

void
mrb_mruby_ev3_gem_final(mrb_state *mrb)
{
}