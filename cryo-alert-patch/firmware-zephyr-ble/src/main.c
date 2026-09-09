#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/pm/pm.h>
#include <zephyr/logging/log.h>

#include "temp_sensor.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

#define SAMPLING_INTERVAL_MS 60000
#define BATTERY_INTERVAL_MS  300000

static const struct device *i2c_dev;

struct temp_data {
    float temperature;
    uint16_t battery_mv;
    uint8_t sequence;
    uint8_t status;
};

static struct temp_data current = {0};

static void bt_ready(int err)
{
    if (err) {
        LOG_ERR("BLE init failed: %d", err);
        return;
    }
    LOG_INF("BLE ready");
}

static void sample_temperature(void)
{
    float temp;
    int ret = temp_sensor_read(i2c_dev, &temp);
    if (ret == 0) {
        current.temperature = temp;
        current.status &= ~0x02;
    } else {
        current.status |= 0x02;
        LOG_WRN("Sensor read failed: %d", ret);
    }
}

static void sample_battery(void)
{
    current.battery_mv = 3000;
}

static void update_ble_advertisement(void)
{
    uint8_t adv_data[] = {
        0x02, 0x01, 0x06,
        0x07, 0x09, 'C', 'r', 'y', 'o', 'P', 'a', 't', 'c', 'h',
    };
    (void)adv_data;
}

void main(void)
{
    int err;

    i2c_dev = DEVICE_DT_GET(DT_NODELABEL(i2c1));
    if (!device_is_ready(i2c_dev)) {
        LOG_ERR("I2C device not ready");
        return;
    }

    err = temp_sensor_init(i2c_dev);
    if (err) {
        LOG_ERR("Temp sensor init failed: %d", err);
        return;
    }

    err = bt_enable(bt_ready);
    if (err) {
        LOG_ERR("BLE enable failed: %d", err);
        return;
    }

    LOG_INF("Alert Patch started");

    while (1) {
        sample_temperature();
        sample_battery();
        current.sequence++;
        update_ble_advertisement();

        k_sleep(K_MSEC(SAMPLING_INTERVAL_MS));
    }
}
