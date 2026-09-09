#include "tmp117.h"

uint8_t TMP117_Init(TMP117_t *s, I2C_HandleTypeDef *hi2c, uint8_t addr) {
    s->hi2c = hi2c;
    s->addr = addr;

    uint16_t devId = 0;
    if (TMP117_ReadDeviceId(s, &devId) != 0) {
        return 1;
    }
    // TMP117 device id is 0x0110 (configurable variant 0x0111)
    if (devId != 0x0110 && devId != 0x0111) {
        return 1;
    }
    return 0;
}

uint8_t TMP117_ReadDeviceId(TMP117_t *s, uint16_t *id) {
    uint8_t reg = TMP117_REG_DEVICE_ID;
    uint8_t data[2];
    if (HAL_I2C_Master_Transmit(s->hi2c, (s->addr << 1), &reg, 1, 100) != HAL_OK) {
        return 1;
    }
    if (HAL_I2C_Master_Receive(s->hi2c, (s->addr << 1), data, 2, 100) != HAL_OK) {
        return 1;
    }
    *id = ((uint16_t)data[0] << 8) | data[1];
    return 0;
}

uint8_t TMP117_ReadTempC(TMP117_t *s, float *tempC) {
    uint8_t reg = TMP117_REG_TEMP;
    uint8_t data[2];
    if (HAL_I2C_Master_Transmit(s->hi2c, (s->addr << 1), &reg, 1, 100) != HAL_OK) {
        return 1;
    }
    if (HAL_I2C_Master_Receive(s->hi2c, (s->addr << 1), data, 2, 100) != HAL_OK) {
        return 1;
    }
    int16_t raw = (int16_t)(((uint16_t)data[0] << 8) | data[1]);
    *tempC = (float)raw * 0.0078125f;
    return 0;
}

uint8_t TMP117_SetMode(TMP117_t *s, uint8_t mode) {
    uint8_t reg = TMP117_REG_CONFIG;
    uint8_t cmd[3] = {reg, 0x00, 0x00};
    if (HAL_I2C_Master_Transmit(s->hi2c, (s->addr << 1), cmd, 3, 100) != HAL_OK) {
        return 1;
    }
    // Re-read config, set MOD bits (10-11), write back
    uint8_t config[2];
    HAL_I2C_Master_Transmit(s->hi2c, (s->addr << 1), &reg, 1, 100);
    HAL_I2C_Master_Receive(s->hi2c, (s->addr << 1), config, 2, 100);
    uint16_t cfg = ((uint16_t)config[0] << 8) | config[1];
    cfg &= ~(0x03 << 10);
    cfg |= ((mode & 0x03) << 10);
    cmd[1] = (uint8_t)(cfg >> 8);
    cmd[2] = (uint8_t)(cfg & 0xFF);
    return (HAL_I2C_Master_Transmit(s->hi2c, (s->addr << 1), cmd, 3, 100) == HAL_OK) ? 0 : 1;
}
