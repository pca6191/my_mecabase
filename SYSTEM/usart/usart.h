#ifndef __USART_H
#define __USART_H

#include <stdint.h>

#define PRINTF_BUFFER_SIZE 256

void usart2_init(uint32_t bound);
void usart2_send_byte(uint8_t ch);
void usart2_send_string(char *str);
void usart2_printf(const char *format, ...);
#endif
