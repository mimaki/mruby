#include <stdio.h>
#include <string.h>
#include "mruby.h"
#include "mruby/value.h"
#include "mruby/variable.h"
#include "mruby/class.h"
#include "mruby/string.h"
#include "mruby/hash.h"

// -----------------------------------

#define EV3_LCD_WIDTH   178
#define EV3_LCD_HEIGHT  128

#define EV3_FONT_SMALL  0   /*  6x 8 pixels */
#define EV3_FONT_MEDIUM 1   /* 10x16 pixels */

#define EV3_LCD_WHITE   0
#define EV3_LCD_BLACK   1

typedef void (*ISR)(intptr_t);

typedef enum {
    LEFT_BUTTON  = 0, //!< \~English Left button       \~Japanese 左ボタン
    RIGHT_BUTTON = 1, //!< \~English Right button    \~Japanese 右ボタン
    UP_BUTTON    = 2, //!< \~English Up button         \~Japanese 上ボタン
    DOWN_BUTTON  = 3, //!< \~English Down button       \~Japanese 下ボタン
    ENTER_BUTTON = 4, //!< \~English Enter button      \~Japanese 中央ボタン
    BACK_BUTTON  = 5, //!< \~English Back button       \~Japanese 戻るボタン
    TNUM_BUTTON  = 6, //!< \~English Number of buttons \~Japanese ボタンの数
} button_t;

typedef enum {
  LED_OFF    = 0,           //!< \~English Turn off \~Japanese オフにする
  LED_RED    = 1 << 0,        //!< \~English Red      \~Japanese 赤
  LED_GREEN  = 1 << 1,        //!< \~English Green    \~Japanese 緑
  LED_ORANGE = LED_RED | LED_GREEN, //!< \~English Orange   \~Japanese オレンジ色
} ledcolor_t;

/* EV3 API wrapper functions */
/* RTOS */
int32_t EV3_delay(int32_t);
/* LCD */
int32_t EV3_lcd_set_font(int16_t);
int32_t EV3_font_get_size(int16_t, int32_t*, int32_t*);
int32_t EV3_lcd_draw_string(const char*, uint32_t, uint32_t);
/* Button */
int32_t EV3_button_is_pressed(int16_t);
int32_t EV3_button_set_on_clicked(int16_t, ISR, intptr_t);
/* Battery */
int EV3_battery_current_mA(void);
int EV3_battery_voltage_mV(void);
/* LED */
int32_t EV3_led_set_color(int16_t);
/* Sound */
int32_t EV3_speaker_play_tone(uint16_t, int32_t);
int32_t EV3_speaker_set_volume(uint8_t);
int32_t EV3_speaker_stop(void);

// -----------------------------------

typedef struct ev3_font_size {
  int32_t w;
  int32_t h;
} ev3_font_size;

static ev3_font_size _font_size[2] = {{0, 8}, {0, 16}};

/*
 *  call-seq:
 *     delay(100)
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
 *     LCD.new(LCD::MEDIUM_FONT, 0, 0, LCD::WIDTH/2, LCD::HEIGHT/2)
 *
 *  Returns LCD object.
 */
static mrb_value
ev3_lcd_init(mrb_state *mrb, mrb_value self)
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
 *     LCD.font = DEFAULT
 *
 *  Returns nil.
 */
static mrb_value
ev3_set_font(mrb_state *mrb, mrb_value self)
{
  mrb_int font;

  mrb_get_args(mrb, "i", &font);

  EV3_lcd_set_font((int16_t)font);
  mrb_cv_set(mrb, self, mrb_intern_lit(mrb, "@@font"), mrb_fixnum_value(font));

  return mrb_nil_value();
}

static void
ev3_lcd_get_font_size(mrb_state *mrb, mrb_value lcd, mrb_int *fw, mrb_int *fh)
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
ev3_lcd_lf(mrb_state *mrb, mrb_value lcd)
{
  mrb_int font    = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@font")));
  mrb_int height  = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@height")));
  mrb_int cy      = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cy")));
  mrb_int ch;

#ifndef EV3
  puts("");
#endif
  ch = height / _font_size[font].h;

  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(0));
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value((cy+1) % ch));
}

static void
ev3_lcd_print_line(mrb_state *mrb, mrb_value lcd, mrb_value *str)
{
  const char *src = mrb_string_value_cstr(mrb, str);
  size_t len = strlen(src);
  char *buf = mrb_malloc(mrb, len+1);
  mrb_int font = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@font")));
  mrb_assert(font == EV3_FONT_SMALL || font == EV3_FONT_MEDIUM);
  mrb_assert(_font_size[font].w != 0 && _font_size[font].h != 0);

  mrb_int x0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@left")));
  mrb_int y0 = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@top")));
  mrb_int cx = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cx")));
  mrb_int cy = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@cy")));
  mrb_int cw = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@width"))) / _font_size[font].w;
  mrb_int ch = mrb_fixnum(mrb_iv_get(mrb, lcd, mrb_intern_lit(mrb, "@height"))) / _font_size[font].h;
  mrb_int fw = _font_size[font].w;
  mrb_int fh = _font_size[font].h;
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
      EV3_lcd_draw_string(buf, x0+cx*fw, y0+cy*fh);

      /* line feed */
#ifndef EV3
  puts("");
#endif
      cy++;
      if (cy >= ch) cy = 0;
      cx = csr = 0;

      memset(buf, 0, strlen(src)+1);
      dst = buf;
    }
  }
  *dst = '\0';
  if (dst != buf) {
    EV3_lcd_draw_string(buf, x0+cx*fw, y0+cy*fh);
  }

  mrb_free(mrb, buf);

  /* update cursor */
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(csr)); 
  mrb_iv_set(mrb, lcd, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(cy)); 
}

/*
 *  call-seq:
 *     LCD.print("abc")
 *
 *  Returns nil.
 */
static mrb_value
ev3_lcd_print(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *args;
  mrb_int i;
  mrb_value s;

  mrb_get_args(mrb, "*", &args, &argc);

  ev3_set_current_font(mrb, self);

  for(i=0; i<argc; i++,args++) {
    s = mrb_funcall(mrb, *args, "to_s", 0);
    ev3_lcd_print_line(mrb, self, &s);
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LCD.puts("abc")
 *
 *  Returns nil.
 */
static mrb_value
ev3_lcd_puts(mrb_state *mrb, mrb_value self)
{
  mrb_int argc;
  mrb_value *args;
  mrb_int i;
  mrb_value s;

  mrb_get_args(mrb, "*", &args, &argc);

  ev3_set_current_font(mrb, self);

  if (argc == 0) {
    ev3_lcd_lf(mrb, self);
  }
  for(i=0; i<argc; i++,args++) {
    s = mrb_funcall(mrb, *args, "to_s", 0);
    ev3_lcd_print_line(mrb, self, &s);
    ev3_lcd_lf(mrb, self);
  }

  return mrb_nil_value();
}

/*
 *  call-seq:
 *     LCD.draw(4, 1, "ABC")
 *
 *  Returns nil.
 */
static mrb_value
ev3_lcd_draw_string(mrb_state *mrb, mrb_value self)
{
  mrb_value obj, str;
  mrb_int x, y;
  mrb_int fw, fh;
  mrb_int cw, ch;

  ev3_lcd_get_font_size(mrb, self, &fw, &fh);
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

static mrb_value
ev3_lcd_cx(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cx"));
}

static mrb_value
ev3_lcd_cy(mrb_state *mrb, mrb_value self)
{
  return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@cy"));
}

static mrb_value
ev3_lcd_locate(mrb_state *mrb, mrb_value self)
{
  mrb_int x, y = -1;

  mrb_get_args(mrb, "i|i", &x, &y);

  mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cx"), mrb_fixnum_value(x));
  if (y >= 0) {
    mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@cy"), mrb_fixnum_value(y));
  }

  return mrb_nil_value();
}

static mrb_value
ev3_btn_init(mrb_state *mrb, mrb_value self)
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

static mrb_value
ev3_btn_get(mrb_state *mrb, mrb_value self)
{
  mrb_sym sym;
  mrb_value symv;

  mrb_get_args(mrb, "n", &sym);
  symv = mrb_symbol_value(sym);

  return mrb_obj_new(mrb, mrb_class_ptr(self), 1, &symv);
}

static mrb_value
ev3_btn_pressed(mrb_state *mrb, mrb_value self)
{
  int32_t pressed = FALSE;
  mrb_value key;

  key = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@key"));
  if (!mrb_nil_p(key)) {
    pressed = EV3_button_is_pressed((int16_t)mrb_fixnum(key));
  }

  return mrb_bool_value(pressed);
}

void
mrb_mruby_ev3_gem_init(mrb_state *mrb)
{
  struct RClass *ev3;
  struct RClass *lcd;
  struct RClass *btn;
  mrb_value lcdo;
  mrb_value keys;

  /* EV3 module */
  ev3 = mrb_define_module(mrb, "EV3");

  /* RTOS api */
  mrb_define_method(mrb, ev3, "delay", rtos_delay, MRB_ARGS_REQ(1));

  /* LCD class */
  lcd = mrb_define_class_under(mrb, ev3, "LCD", mrb->object_class);
  lcdo = mrb_obj_value(lcd);

  /* constants */
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "WIDTH"),        mrb_fixnum_value(EV3_LCD_WIDTH));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "HEIGHT"),       mrb_fixnum_value(EV3_LCD_HEIGHT));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "SMALL_FONT"),   mrb_fixnum_value(EV3_FONT_SMALL));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "MEDIUM_FONT"),  mrb_fixnum_value(EV3_FONT_MEDIUM));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "WHITE"),        mrb_fixnum_value(EV3_LCD_WHITE));
  mrb_const_set(mrb, lcdo, mrb_intern_lit(mrb, "BLACK"),        mrb_fixnum_value(EV3_LCD_BLACK));

  mrb_mod_cv_set(mrb, lcd, mrb_intern_lit(mrb, "@@font"),       mrb_fixnum_value(EV3_FONT_SMALL));

  mrb_define_class_method(mrb, lcd, "font=", ev3_set_font,        MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, lcd, "draw",  ev3_lcd_draw_string, MRB_ARGS_REQ(3));

  mrb_define_method(mrb, lcd, "initialize", ev3_lcd_init,   MRB_ARGS_OPT(6));
  mrb_define_method(mrb, lcd, "print",      ev3_lcd_print,  MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "puts",       ev3_lcd_puts,   MRB_ARGS_ANY());
  mrb_define_method(mrb, lcd, "cx",         ev3_lcd_cx,     MRB_ARGS_NONE());
  mrb_define_method(mrb, lcd, "cy",         ev3_lcd_cy,     MRB_ARGS_NONE());
  mrb_define_method(mrb, lcd, "locate",     ev3_lcd_locate, MRB_ARGS_ARG(1, 1));

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

  mrb_define_class_method(mrb, btn, "[]", ev3_btn_get, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, btn, "initialize", ev3_btn_init,     MRB_ARGS_REQ(1));
  mrb_define_method(mrb, btn, "pressed?",   ev3_btn_pressed,  MRB_ARGS_NONE());

  /* Object includes EV3 */
  mrb_include_module(mrb, mrb->object_class, ev3);
}

void
mrb_mruby_ev3_gem_final(mrb_state *mrb)
{
}