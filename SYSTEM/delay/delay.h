#ifndef __DELAY_H
#define __DELAY_H
#include <stdint.h>

void delay_init(uint8_t SYSCLK);
void delay_ms(uint16_t nms);

#endif
