#include <stdbool.h>
#include <stdio.h>

#include "motor.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "IOI2C.h"
#include "mpu6050.h"


int main(void)
{
  delay_init(168);
  usart2_init(9600);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  LED_Init();

  motor_pwm_init(8399, 1);  // PWM 10KHZ
  motor_init();  // init I/O

  delay_ms(500);
  IIC_Init();
  MPU6050_initialize();
  DMP_Init();

  while (true) {
    delay_ms(300);
    Led_toggle();
    // usart2_send_byte('h');
    // usart2_send_string("I love you\n");
    MPU6050_getDeviceID();
  }
}
