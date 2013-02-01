#include <stdio.h>
#include "mruby.h"

#ifdef ENZI_DEBUG

// DigitalIO
void LEDon()
{
  printf("LEDon() called.\n");
}

void LEDoff()
{
  printf("LEDoff() called.\n");
}

// AnalogIO
//int32_t _AnalogRead(int32_t);

void AnalogWrite(int32_t pin, int32_t duty)
{
  printf("AnalogWrite(%d, %d) called.\n", pin, duty);
}

#endif /* ENZI_DEBUG */
