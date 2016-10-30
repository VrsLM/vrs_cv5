/*
 * vrs_cv5.c
 *
 *  Created on: 18. 10. 2016
 *      Author: Lukas
 */
#include "vrs_cv5.h"

uint16_t AD_value;
uint8_t pom = 1;
uint8_t message = 0;

char pole[10];
int intNum;
int floatNum;

void gpio_init(void) {
	/* Enable GPIO clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure ADCx Channel 0 as analog input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Configure USART pins */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
}

void adc_init(void) {
	// Enable ADC clock 	//zapn˙ù hodiny
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* Enable the HSI oscillator */
	RCC_HSICmd(ENABLE);
	/* Check that HSI oscillator is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		;

	/* Initialize ADC structure */
	ADC_InitTypeDef ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);

	/* ADC1 configuration */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // povolenie continue modu .. ked sa zapne bude zapaty
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADCx regular channel8 configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_384Cycles);

	//Povolenie preruöenia na EOC a OVR
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	//nastavenie NVIC preruöenia
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 0 najvacsia priorita
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the ADC */
	ADC_Cmd(ADC1, ENABLE);

	/* Wait until the ADC1 is ready */
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
		;

	/* Start ADC Software Conversion */
	ADC_SoftwareStartConv(ADC1);
}

void usart_init(void) {
	//nastavenie ötrukt˙ry
	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9800;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //USART_IT_RXNE: Receive Data register not empty interrupt.

	//nastavenie ötrukt˙ry
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //cim vacsie cislo tym mensia priorita
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);

	USART_Cmd(USART2, ENABLE);
}

void ADC1_IRQHandler(void) {
	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)) {
		AD_value = ADC_GetConversionValue(ADC1);
	}
}

void USART2_IRQHandler(void) {
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE)) {
		message = USART_ReceiveData(USART2);

		if (message == 'm') {
			pom = pom == 1 ? 0 : 1;
		}
	}
}

void sendData() {
	if (pom) {
		intNum = AD_value * 3.3 / 4095;
		floatNum = AD_value * 330 / 4095 - intNum * 100;
		sprintf(pole, "%d.%d V\r\n", intNum, floatNum);
	} else {
		sprintf(pole, "%d\r\n", AD_value);
	}

	int i = 0;
	while (pole[i] != 0) {
		USART_SendData(USART2, (char) pole[i++]);
		while (!USART_GetFlagStatus(USART2, USART_FLAG_TC))
			;
	}
}

void delay(int i) {
	for (int x = 0; x < i; x++) {
	}
}
