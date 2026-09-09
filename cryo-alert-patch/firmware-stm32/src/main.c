#include "stm32l0xx_hal.h"

#include "tmp117.h"

/* Pin mapping (matches KiCad v6 netlist) */
#define LED_PIN      GPIO_PIN_3   /* PB3 -> LED_SIG */
#define LED_PORT     GPIOB
#define BUZZER_PIN   GPIO_PIN_8   /* PA8 -> BUZZER */
#define BUZZER_PORT  GPIOA
#define BUTTON_PIN   GPIO_PIN_4   /* PB4 -> SW_SIG */
#define BUTTON_PORT  GPIOB
#define VBAT_PIN     GPIO_PIN_0   /* PA0 -> VBAT_SENSE (ADC) */
#define VBAT_PORT    GPIOA

/* Thresholds */
#define TEMP_HIGH_C   38.5f
#define TEMP_LOW_C    34.0f
#define TEMP_CRIT_HI  42.0f
#define TEMP_CRIT_LO  30.0f
#define VBAT_LOW      2.2f

/* Timing (ms) */
#define MEASURE_PERIOD     2000
#define BLINK_NORMAL       2000
#define BLINK_WARNING      500
#define BLINK_ALERT        200
#define MUTE_DURATION      30000
#define DEBOUNCE           50

typedef enum {
    STATE_NORMAL = 0,
    STATE_WARNING,
    STATE_CRITICAL_ALERT,
    STATE_MUTED
} SystemState;

static SystemState state = STATE_NORMAL;
static uint32_t lastMeasure = 0;
static uint32_t lastBlink = 0;
static uint32_t muteStart = 0;
static uint8_t ledOn = 0;

static I2C_HandleTypeDef hi2c;
static ADC_HandleTypeDef hadc;
static TMP117_t sensor;

static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_ADC_Init(void);
static void Error_Handler(void);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_ADC_Init();

    if (TMP117_Init(&sensor, &hi2c, TMP117_I2C_ADDR) != 0) {
        /* Sensor not found - keep monitoring, LED solid (indicates fault) */
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
    } else {
        TMP117_SetMode(&sensor, 0x00); /* continuous conversion */
    }

    while (1) {
        uint32_t now = HAL_GetTick();

        /* Button (mute/snooze) with debounce */
        if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET) {
            HAL_Delay(DEBOUNCE);
            if (HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN) == GPIO_PIN_RESET) {
                if (state == STATE_CRITICAL_ALERT || state == STATE_WARNING) {
                    state = STATE_MUTED;
                    muteStart = now;
                    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                }
            }
        }

        if (state == STATE_MUTED && (now - muteStart) > MUTE_DURATION) {
            state = STATE_NORMAL;
        }

        /* Periodic measurement */
        if ((now - lastMeasure) >= MEASURE_PERIOD) {
            lastMeasure = now;

            float temp = 0;
            if (TMP117_ReadTempC(&sensor, &temp) != 0) {
                temp = 0; /* sensor error -> treat as critical */
            }

            /* VBAT via ADC */
            HAL_ADC_Start(&hadc);
            HAL_ADC_PollForConversion(&hadc, 10);
            uint16_t adcRaw = HAL_ADC_GetValue(&hadc);
            HAL_ADC_Stop(&hadc);
            float vbat = (adcRaw * 3.0f / 4095.0f); /* 3.0V ADC ref; scaling TBD */

            if (state != STATE_MUTED) {
                if (temp < TEMP_CRIT_LO || temp > TEMP_CRIT_HI || vbat < VBAT_LOW) {
                    state = STATE_CRITICAL_ALERT;
                } else if (temp < TEMP_LOW_C || temp > TEMP_HIGH_C) {
                    state = STATE_WARNING;
                } else {
                    state = STATE_NORMAL;
                }
            }
        }

        /* Actuators */
        switch (state) {
            case STATE_NORMAL:
                if ((now - lastBlink) >= BLINK_NORMAL) {
                    lastBlink = now;
                    ledOn = !ledOn;
                    HAL_GPIO_WritePin(LED_PORT, LED_PIN, ledOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
                }
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                break;

            case STATE_WARNING:
                if ((now - lastBlink) >= BLINK_WARNING) {
                    lastBlink = now;
                    ledOn = !ledOn;
                    HAL_GPIO_WritePin(LED_PORT, LED_PIN, ledOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
                }
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                break;

            case STATE_CRITICAL_ALERT:
                if ((now - lastBlink) >= BLINK_ALERT) {
                    lastBlink = now;
                    ledOn = !ledOn;
                    HAL_GPIO_WritePin(LED_PORT, LED_PIN, ledOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, ledOn ? GPIO_PIN_SET : GPIO_PIN_RESET);
                }
                break;

            case STATE_MUTED:
                HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
                HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
                break;
        }
    }
}

static void MX_I2C1_Init(void) {
    hi2c.Instance = I2C1;
    hi2c.Init.Timing = 0x2000090E; /* 100kHz */
    hi2c.Init.OwnAddress1 = 0;
    hi2c.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
        Error_Handler();
    }
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c, 0) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_ADC_Init(void) {
    hadc.Instance = ADC1;
    hadc.Init.OversamplingMode = DISABLE;
    hadc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc.Init.Resolution = ADC_RESOLUTION_12B;
    hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
    hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
    hadc.Init.LowPowerAutoWait = DISABLE;
    hadc.Init.LowPowerAutoPowerOff = DISABLE;
    hadc.Init.ContinuousConvMode = DISABLE;
    hadc.Init.DiscontinuousConvMode = DISABLE;
    hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc.Init.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;
    if (HAL_ADC_Init(&hadc) != HAL_OK) {
        Error_Handler();
    }
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_0; /* PA0 */
    sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* LED (PB3) output */
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Buzzer (PA8) output */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Button (PB4) input pull-up */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* VBAT sense (PA0) analog */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
        Error_Handler();
    }
}

static void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}
