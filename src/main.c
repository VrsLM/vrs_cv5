/* Includes */
#include <stddef.h>
#include <stdio.h>
#include "stm32l1xx.h"
#include "vrs_cv5.h"

int value;
uint16_t  AD_value;

int main(void) {
	led_init();
	adc_init();
	usart_init();

	int value;

	while (1) {
		value = readValue();

		if (value < 2050) {
			delay(50000);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		} else if (2800 < value && value < 3000) {
			delay(120000);
			GPIO_ToggleBits(GPIOA, GPIO_Pin_5);
		}
	}
	return 0;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/*
 * Minimal __assert_func used by the assert() macro
 * */
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
 * Minimal __assert() uses __assert__func()
 * */
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
