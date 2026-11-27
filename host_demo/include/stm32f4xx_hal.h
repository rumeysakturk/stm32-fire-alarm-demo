#ifndef HOST_DEMO_STM32F4XX_HAL_H
#define HOST_DEMO_STM32F4XX_HAL_H

#include <stdint.h>

/* Minimal GPIO surface used by fire_alarm.c for a desktop-only demonstration. */
typedef struct {
    uint16_t output_state;
} GPIO_TypeDef;

typedef enum {
    GPIO_PIN_RESET = 0,
    GPIO_PIN_SET = 1
} GPIO_PinState;

extern GPIO_TypeDef host_gpio_a;
extern GPIO_TypeDef host_gpio_b;
extern GPIO_TypeDef host_gpio_c;

#define GPIOA (&host_gpio_a)
#define GPIOB (&host_gpio_b)
#define GPIOC (&host_gpio_c)

#define GPIO_PIN_0  ((uint16_t)0x0001U)
#define GPIO_PIN_5  ((uint16_t)0x0020U)
#define GPIO_PIN_13 ((uint16_t)0x2000U)

void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state);

#endif
