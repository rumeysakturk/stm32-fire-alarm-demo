#include "main.h"
#include "fire_alarm.h"
#include <stdio.h>

ADC_HandleTypeDef hadc1;
UART_HandleTypeDef huart2;
TIM_HandleTypeDef htim2;

static volatile uint8_t measurement_due;
static uint32_t button_candidate_since;
static GPIO_PinState button_candidate_state = GPIO_PIN_RESET;
static GPIO_PinState button_stable_state = GPIO_PIN_RESET;
static bool button_initialized;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static bool ReadSensors(FireSensorSample *sample);
static void ReportSample(const FireSensorSample *sample);
static void HandleResetButton(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_ADC1_Init();
    MX_USART2_UART_Init();
    MX_TIM2_Init();
    FireAlarm_Init();

    (void)HAL_UART_Transmit(&huart2, (uint8_t *)"Fire alarm demo ready\r\n",
                           (uint16_t)(sizeof("Fire alarm demo ready\r\n") - 1U), 100U);
    if (HAL_TIM_Base_Start_IT(&htim2) != HAL_OK) {
        Error_Handler();
    }

    while (1) {
        HandleResetButton();
        if (measurement_due != 0U) {
            measurement_due = 0U;
            FireSensorSample sample = {0U, 0U, false};
            sample.valid = ReadSensors(&sample);
            FireAlarm_ProcessSample(&sample);
            ReportSample(&sample);
        }
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};

    osc.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc.HSIState = RCC_HSI_ON;
    osc.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    osc.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&osc) != HAL_OK) Error_Handler();

    clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    clk.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clk.APB1CLKDivider = RCC_HCLK_DIV1;
    clk.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_0) != HAL_OK) Error_Handler();
}

static void MX_ADC1_Init(void)
{
    ADC_ChannelConfTypeDef channel = {0};

    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DMAContinuousRequests = DISABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    if (HAL_ADC_Init(&hadc1) != HAL_OK) Error_Handler();

    channel.Channel = ADC_CHANNEL_0;
    channel.Rank = 1;
    channel.SamplingTime = ADC_SAMPLETIME_84CYCLES;
    if (HAL_ADC_ConfigChannel(&hadc1, &channel) != HAL_OK) Error_Handler();
    channel.Channel = ADC_CHANNEL_1;
    channel.Rank = 2;
    if (HAL_ADC_ConfigChannel(&hadc1, &channel) != HAL_OK) Error_Handler();
}

static void MX_USART2_UART_Init(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 115200;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart2) != HAL_OK) Error_Handler();
}

static void MX_TIM2_Init(void)
{
    htim2.Instance = TIM2;
    htim2.Init.Prescaler = 15999;
    htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim2.Init.Period = 999;
    htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
    if (HAL_TIM_Base_Init(&htim2) != HAL_OK) Error_Handler();
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio = {0};
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);

    gpio.Pin = STATUS_LED_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(STATUS_LED_GPIO_Port, &gpio);

    gpio.Pin = BUZZER_Pin;
    gpio.Mode = GPIO_MODE_OUTPUT_PP;
    gpio.Pull = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BUZZER_GPIO_Port, &gpio);

    gpio.Pin = RESET_BTN_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(RESET_BTN_GPIO_Port, &gpio);

    gpio.Pin = SMOKE_ADC_Pin | TEMP_ADC_Pin;
    gpio.Mode = GPIO_MODE_ANALOG;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);
}

static bool ReadAdcPair(uint32_t *first, uint32_t *second)
{
    if (HAL_ADC_Start(&hadc1) != HAL_OK) return false;
    if (HAL_ADC_PollForConversion(&hadc1, 50U) != HAL_OK) {
        (void)HAL_ADC_Stop(&hadc1);
        return false;
    }
    *first = HAL_ADC_GetValue(&hadc1);
    if (HAL_ADC_PollForConversion(&hadc1, 50U) != HAL_OK) {
        (void)HAL_ADC_Stop(&hadc1);
        return false;
    }
    *second = HAL_ADC_GetValue(&hadc1);
    return HAL_ADC_Stop(&hadc1) == HAL_OK;
}

static bool ReadSensors(FireSensorSample *sample)
{
#if FIRE_ALARM_SIMULATION
    static uint32_t simulation_step;
    ++simulation_step;
    sample->valid = true;
    sample->smoke_raw = 500U;
    sample->temperature_raw = 600U;
    if ((simulation_step >= 5U) && (simulation_step <= 8U)) sample->smoke_raw = 3000U;
    if ((simulation_step >= 15U) && (simulation_step % 10U == 0U)) sample->valid = false;
    return sample->valid;
#else
    uint32_t smoke = 0U;
    uint32_t temperature = 0U;
    /* Read rank 1 and rank 2 from the same scan sequence. */
    if (!ReadAdcPair(&smoke, &temperature)) return false;
    if ((smoke > 4095U) || (temperature > 4095U)) return false;
    sample->smoke_raw = (uint16_t)smoke;
    sample->temperature_raw = (uint16_t)temperature;
    return true;
#endif
}

static void ReportSample(const FireSensorSample *sample)
{
    char line[96];
    int length;
    if (sample->valid) {
        length = snprintf(line, sizeof(line), "smoke=%u temp=%u state=%s\r\n",
                          sample->smoke_raw, sample->temperature_raw,
                          FireAlarm_StateName(FireAlarm_GetState()));
    } else {
        length = snprintf(line, sizeof(line), "sensor=INVALID state=%s\r\n",
                          FireAlarm_StateName(FireAlarm_GetState()));
    }
    if ((length > 0) && ((size_t)length < sizeof(line))) {
        (void)HAL_UART_Transmit(&huart2, (uint8_t *)line, (uint16_t)length, 100U);
    }
}

static void HandleResetButton(void)
{
    const GPIO_PinState current = HAL_GPIO_ReadPin(RESET_BTN_GPIO_Port, RESET_BTN_Pin);
    const uint32_t now = HAL_GetTick();
    if (!button_initialized) {
        button_candidate_state = current;
        button_stable_state = current;
        button_candidate_since = now;
        button_initialized = true;
        return;
    }

    if (current != button_candidate_state) {
        button_candidate_state = current;
        button_candidate_since = now;
    } else if ((current != button_stable_state) &&
               ((now - button_candidate_since) >= 30U)) {
        button_stable_state = current;
        if (button_stable_state == GPIO_PIN_SET) {
            FireAlarm_RequestReset();
        }
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) measurement_due = 1U;
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1) {
        __HAL_RCC_ADC1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2) {
        GPIO_InitTypeDef gpio = {0};
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        gpio.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Pull = GPIO_PULLUP;
        gpio.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio.Alternate = GPIO_AF7_USART2;
        HAL_GPIO_Init(GPIOA, &gpio);
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        __HAL_RCC_TIM2_CLK_ENABLE();
        HAL_NVIC_SetPriority(TIM2_IRQn, 1U, 0U);
        HAL_NVIC_EnableIRQ(TIM2_IRQn);
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}
