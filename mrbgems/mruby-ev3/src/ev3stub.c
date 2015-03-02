#include <stdio.h>
#include <stdint.h>

#ifndef EV3
int32_t
EV3_delay(int32_t tmo)
{
  return 0;
}

int32_t
EV3_lcd_set_font(int16_t font)
{
  return 0;
}

int32_t
EV3_font_get_size(int16_t font, int32_t *pwidth, int32_t *pheight)
{
  switch(font) {
  case 0:
    *pwidth  = 6;
    *pheight = 8;
    break;

  case 1:
  default:
    *pwidth  = 10;
    *pheight = 16;
    break;
  }
  return 0;
}

int32_t
EV3_lcd_draw_string(const char *buf, uint32_t x, uint32_t y)
{
  printf("%s", buf);
  return 0;
}

int32_t
EV3_button_is_pressed(int16_t key)
{
  return 0;
}
#endif
