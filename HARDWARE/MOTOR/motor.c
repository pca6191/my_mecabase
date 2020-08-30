/*
 * motor.c
 *
 *  Created on: Aug 29, 2020
 *      Author: pca
 */
#include <stm32f4xx.h>

#include "motor.h"

void motor_pwm_init(uint16_t arr,uint16_t psc)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;

  // init timer8 / gpio clocks
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  // 將 GPIO 綁定到 timer8 output
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_TIM8);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_TIM8);

  // 設定 gpio 基本屬性
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;  // GPIOC6.7.8.9
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // 設定 timer 屬性: Tout=((arr+1)*(psc+1))/Tclk
  //  ==> Tout = (8399 + 1)*(1+1) / 168M = 16800 / 168000K = 1/10K (sec)
  TIM_TimeBaseStructure.TIM_Period = arr;
  TIM_TimeBaseStructure.TIM_Prescaler = psc;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  // PWM1: count 超過 threshold 則 output low
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM8, &TIM_OCInitStructure);
  TIM_OC2Init(TIM8, &TIM_OCInitStructure);
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);
  TIM_OC4Init(TIM8, &TIM_OCInitStructure);

  TIM_CtrlPWMOutputs(TIM8, ENABLE);

  TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);
  TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM8, ENABLE);

  TIM_Cmd(TIM8, ENABLE);
}

void motor_init()
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  //GPIO B0.1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //GPIOB0.1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;  //GPIOC4.5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;  //GPIOC4.5
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIOB->BSRRL = GPIO_Pin_0;
  GPIOB->BSRRL = GPIO_Pin_1;
  GPIOC->BSRRL = GPIO_Pin_4;
  GPIOC->BSRRL = GPIO_Pin_5;
}
