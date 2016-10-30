/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Lukas
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include "stm32l1xx.h"

void adc_init(void);
void gpio_init(void);
void usart_init(void);

void sendData();
void delay(int);

void ADC1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif /* VRS_CV5_H_ */
