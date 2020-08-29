#include <stdbool.h>
#include "delay.h"
#include "led.h"

int main(void)
{
  delay_init(168);
  LED_Init();

  while (true) {
    delay_ms(300);
    Led_toggle();
  }
}
