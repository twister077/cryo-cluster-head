#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>

#include "temp_sensor.h"

int temp_sensor_init(const struct device *i2c_dev)
{
    uint8_t cfg = 0x00;
    return i2c_reg_write_byte(i2c_dev, MAX30205_ADDR, MAX30205_REG_CFG, cfg);
}

int temp_sensor_read(const struct device *i2c_dev, float *temp)
{
    uint8_t buf[2];
    int ret = i2c_reg_read_byte(i2c_dev, MAX30205_ADDR, MAX30205_REG_TEMP, &buf[0]);
    if (ret) {
        return ret;
    }

    ret = i2c_reg_read_byte(i2c_dev, MAX30205_ADDR, MAX30205_REG_TEMP + 1, &buf[1]);
    if (ret) {
        return ret;
    }

    uint16_t raw = ((uint16_t)buf[0] << 8) | buf[1];
    raw >>= 3;
    *temp = raw * 0.03125f;

    return 0;
}
