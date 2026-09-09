#ifndef TMP117_H
#define TMP117_H

#include "stm32l0xx_hal.h"

#define TMP117_I2C_ADDR 0x48  // Default I2C address

// TMP117 Registers
#define TMP117_REG_TEMP      0x00
#define TMP117_REG_CONFIG    0x01
#define TMP117_REG_DEVICE_ID 0x0F

#define TMP117_DEVICE_ID     0x0110

typedef struct {
    I2C_HandleTypeDef *hi2c;
    uint8_t addr;
} TMP117_t;

uint8_t TMP117_Init(TMP117_t *s, I2C_HandleTypeDef *hi2c, uint8_t addr);
uint8_t TMP117_ReadDeviceId(TMP117_t *s, uint16_t *id);
uint8_t TMP117_ReadTempC(TMP117_t *s, float *tempC);
uint8_t TMP117_SetMode(TMP117_t *s, uint8_t mode);

#endif
