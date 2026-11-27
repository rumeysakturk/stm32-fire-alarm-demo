#include "fire_alarm.h"
#include "main.h"

static FireAlarmState alarm_state = FIRE_STATE_NORMAL;
static uint8_t consecutive_high_samples;
static bool alarm_latched;
static bool hazard_present;
static bool latest_sample_valid;

static void apply_outputs(void)
{
    /* Keep a previously latched alarm active if a later sample faults. */
    const bool alarm_output = alarm_latched &&
                              ((alarm_state == FIRE_STATE_ALARM) ||
                               (alarm_state == FIRE_STATE_FAULT));
    const GPIO_PinState output = alarm_output ? GPIO_PIN_SET : GPIO_PIN_RESET;
    HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, output);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, output);
}

void FireAlarm_Init(void)
{
    alarm_state = FIRE_STATE_NORMAL;
    consecutive_high_samples = 0U;
    alarm_latched = false;
    hazard_present = false;
    latest_sample_valid = false;
    apply_outputs();
}

void FireAlarm_ProcessSample(const FireSensorSample *sample)
{
    if ((sample == NULL) || !sample->valid ||
        (sample->smoke_raw > 4095U) || (sample->temperature_raw > 4095U)) {
        latest_sample_valid = false;
        hazard_present = false;
        consecutive_high_samples = 0U;
        alarm_state = FIRE_STATE_FAULT;
        apply_outputs();
        return;
    }

    latest_sample_valid = true;
    hazard_present = (sample->smoke_raw >= FIRE_ALARM_SMOKE_THRESHOLD) ||
                     (sample->temperature_raw >= FIRE_ALARM_TEMP_THRESHOLD);

    if (hazard_present) {
        if (consecutive_high_samples < FIRE_ALARM_CONFIRM_SAMPLES) {
            ++consecutive_high_samples;
        }
        if (consecutive_high_samples >= FIRE_ALARM_CONFIRM_SAMPLES) {
            alarm_latched = true;
        }
    } else {
        consecutive_high_samples = 0U;
    }

    alarm_state = alarm_latched ? FIRE_STATE_ALARM : FIRE_STATE_NORMAL;
    apply_outputs();
}

void FireAlarm_RequestReset(void)
{
    /* Reset is accepted only with a valid, currently safe sensor sample. */
    if (latest_sample_valid && !hazard_present) {
        alarm_latched = false;
        consecutive_high_samples = 0U;
        alarm_state = FIRE_STATE_NORMAL;
        apply_outputs();
    }
}

FireAlarmState FireAlarm_GetState(void)
{
    return alarm_state;
}

bool FireAlarm_IsHazardPresent(void)
{
    return hazard_present;
}

const char *FireAlarm_StateName(FireAlarmState state)
{
    switch (state) {
    case FIRE_STATE_NORMAL: return "NORMAL";
    case FIRE_STATE_ALARM:  return "ALARM";
    case FIRE_STATE_FAULT:  return "FAULT";
    default:                return "UNKNOWN";
    }
}
