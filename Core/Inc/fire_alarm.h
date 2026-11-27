#ifndef FIRE_ALARM_H
#define FIRE_ALARM_H

#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef FIRE_ALARM_SIMULATION
#define FIRE_ALARM_SIMULATION 1
#endif

#define FIRE_ALARM_SMOKE_THRESHOLD 2600U
#define FIRE_ALARM_TEMP_THRESHOLD  2400U
#define FIRE_ALARM_CONFIRM_SAMPLES 3U

typedef enum {
    FIRE_STATE_NORMAL = 0,
    FIRE_STATE_ALARM,
    FIRE_STATE_FAULT
} FireAlarmState;

typedef struct {
    uint16_t smoke_raw;
    uint16_t temperature_raw;
    bool valid;
} FireSensorSample;

void FireAlarm_Init(void);
void FireAlarm_ProcessSample(const FireSensorSample *sample);
void FireAlarm_RequestReset(void);
FireAlarmState FireAlarm_GetState(void);
bool FireAlarm_IsHazardPresent(void);
const char *FireAlarm_StateName(FireAlarmState state);

#endif
