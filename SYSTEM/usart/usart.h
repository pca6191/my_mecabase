#ifndef __USART_H
#define __USART_H

#include <stdint.h>

void usart2_init(uint32_t bound);
void usart2_send_byte(uint8_t ch);

#endif
