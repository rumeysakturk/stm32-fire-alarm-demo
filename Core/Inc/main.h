#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f4xx_hal.h"

/* NUCLEO-F401RE starter pin map. Recheck against the selected board. */
#define SMOKE_ADC_GPIO_Port GPIOA
#define SMOKE_ADC_Pin       GPIO_PIN_0
#define TEMP_ADC_GPIO_Port  GPIOA
#define TEMP_ADC_Pin        GPIO_PIN_1
#define STATUS_LED_GPIO_Port GPIOA
#define STATUS_LED_Pin      GPIO_PIN_5
#define BUZZER_GPIO_Port    GPIOB
#define BUZZER_Pin          GPIO_PIN_0
#define RESET_BTN_GPIO_Port GPIOC
#define RESET_BTN_Pin       GPIO_PIN_13

void Error_Handler(void);

#endif
