/*
 * motor.h
 *
 *  Created on: Aug 29, 2020
 *      Author: pca
 */

#ifndef __MOTOR_H
#define __MOTOR_H

#include <stdint.h>

void motor_pwm_init(uint16_t arr,uint16_t psc);
void motor_init();

#endif
