#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <zephyr/device.h>

#define MAX30205_ADDR     0x48
#define MAX30205_REG_TEMP 0x00
#define MAX30205_REG_CFG  0x01

int temp_sensor_init(const struct device *i2c_dev);
int temp_sensor_read(const struct device *i2c_dev, float *temp);

#endif
