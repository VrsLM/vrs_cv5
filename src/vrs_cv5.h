/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Lukas
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

#include <stddef.h>
#include "stm32l1xx.h"

void delay(int);
void usart_init(void);

uint16_t readValue(void);
void adc_init(void);
void USART2_IRQHandler();
void ADC1_IRQHandler();

#endif /* VRS_CV5_H_ */
