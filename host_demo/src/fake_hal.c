#include "stm32f4xx_hal.h"

GPIO_TypeDef host_gpio_a = {0U};
GPIO_TypeDef host_gpio_b = {0U};
GPIO_TypeDef host_gpio_c = {0U};

void HAL_GPIO_WritePin(GPIO_TypeDef *port, uint16_t pin, GPIO_PinState state)
{
    if (state == GPIO_PIN_SET) {
        port->output_state |= pin;
    } else {
        port->output_state &= (uint16_t)~pin;
    }
}
