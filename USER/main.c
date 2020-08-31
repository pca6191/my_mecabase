#include <stdbool.h>
#include <stdio.h>

#include "motor.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "IOI2C.h"
#include "mpu6050.h"
#include "exti.h"

int EXTI15_10_IRQHandler(void)
{
  static int cnt = 0;

  if (INT == 0) {
    // do something
    cnt++;
    Read_DMP();

    if(cnt == 100) {
      cnt = 0;
      Led_toggle();

      float r, p, y;
      mpu_get_rpy(&r, &p, &y);
      usart2_printf("roll: %f, pitch: %f, yaw: %f\n", r, p, y);
    }
  }
  else {
    // do something
  }

  return 0;
}

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
  mpu_EXTI_Init();

  while (true) {
    delay_ms(500);
    // Led_toggle();
#if 0  // test usart2
    // usart2_send_byte('h');
    // usart2_send_string("I love you\n");
#endif

#if 0 // test mpu
    MPU6050_getDeviceID();
#endif

#if 0  // test printf
    int i = 14;
    float f = 4.56;
    double db = 12.3;

    usart2_printf("dec: %d\n", i);
    usart2_printf("hex: %02X\n", i);
    usart2_printf("float: %f\n", f);
    usart2_printf("double: %lf\n", db);

#endif

#if 0
    float r, p, y;
    mpu_get_rpy(&r, &p, &y);
    usart2_printf("roll: %f, pitch: %f, yaw: %f\n", r, p, y);
#endif
  }
}
