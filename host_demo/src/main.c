#include "fire_alarm.h"
#include "main.h"
#include <stdio.h>

static void show_sample(const char *label, uint16_t smoke, uint16_t temperature, bool valid)
{
    FireSensorSample sample = {smoke, temperature, valid};
    FireAlarm_ProcessSample(&sample);
    printf("%-18s smoke=%4u temp=%4u valid=%s state=%-6s LED=%s buzzer=%s\n",
           label, smoke, temperature, valid ? "yes" : "no",
           FireAlarm_StateName(FireAlarm_GetState()),
           (host_gpio_a.output_state & STATUS_LED_Pin) ? "ON" : "OFF",
           (host_gpio_b.output_state & BUZZER_Pin) ? "ON" : "OFF");
}

int main(void)
{
    FireAlarm_Init();
    puts("STM32 fire alarm logic demo (desktop simulation)");
    show_sample("normal", 500U, 600U, true);
    show_sample("first high sample", 3000U, 600U, true);
    show_sample("second high sample", 3000U, 600U, true);
    show_sample("third high sample", 3000U, 600U, true);
    show_sample("sensor fault", 0U, 0U, false);
    show_sample("safe again; latched", 500U, 600U, true);
    FireAlarm_RequestReset();
    printf("%-18s state=%-6s LED=%s buzzer=%s\n", "safe reset",
           FireAlarm_StateName(FireAlarm_GetState()),
           (host_gpio_a.output_state & STATUS_LED_Pin) ? "ON" : "OFF",
           (host_gpio_b.output_state & BUZZER_Pin) ? "ON" : "OFF");
    return 0;
}
