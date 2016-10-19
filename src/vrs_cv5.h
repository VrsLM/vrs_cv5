/*
 * vrs_cv5.h
 *
 *  Created on: 18. 10. 2016
 *      Author: Lukas
 */

#ifndef VRS_CV5_H_
#define VRS_CV5_H_

//void setNVIC(void);
void delay(int);
void usart_init(void);

uint16_t readValue(void);
void adc_init(void);

#endif /* VRS_CV5_H_ */
