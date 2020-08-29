#include <stdbool.h>
#include <stdio.h>
#include "delay.h"
#include "led.h"
#include "usart.h"

int main(void)
{
  delay_init(168);
  usart2_init(9600);
  LED_Init();

  while (true) {
    delay_ms(300);
    Led_toggle();
    // usart2_send_byte('h');
    usart2_send_string("I love you\n");
  }
}
