#include <zephyr/kernel.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/logging/log.h>

#include "levels.h"
#include "temp_voter.h"
#include "peltier.h"
#include "safety.h"
#include "auto_mode.h"
#include "ble_patch.h"
#include "ble_phone.h"

LOG_MODULE_REGISTER(main, CONFIG_LOG_DEFAULT_LEVEL);

static const struct device *local_i2c;
static int current_mode = MODE_OFF;
static bool cooling_active = false;

#define LOCAL_SENSOR_ADDR 0x48

#define CONTROL_INTERVAL_MS 5000

static int read_local_temp(float *temp)
{
    uint8_t buf[2];
    int ret = i2c_reg_read_byte(local_i2c, LOCAL_SENSOR_ADDR, 0x00, &buf[0]);
    if (ret) return ret;
    ret = i2c_reg_read_byte(local_i2c, LOCAL_SENSOR_ADDR, 0x01, &buf[1]);
    if (ret) return ret;

    uint16_t raw = ((uint16_t)buf[0] << 8) | buf[1];
    raw >>= 3;
    *temp = raw * 0.03125f;
    return 0;
}

static void refresh_temp_sources(void)
{
    float t;

    if (ble_patch_get_temp(&t) == 0) {
        temp_voter_feed(TEMP_SOURCE_PATCH_BLE, t);
    }

    if (ble_phone_get_temp(&t) == 0) {
        temp_voter_feed(TEMP_SOURCE_PHONE_RELAY, t);
    }

    if (read_local_temp(&t) == 0) {
        temp_voter_feed(TEMP_SOURCE_LOCAL, t);
    }
}

static void update_cooling(void)
{
    struct temp_vote_result vote = temp_voter_get_vote();
    int phone_mode = ble_phone_get_mode();

    if (!vote.safe) {
        LOG_WRN("No temp quorum — safe shutdown");
        peltier_off();
        cooling_active = false;
        current_mode = MODE_OFF;
        return;
    }

    if (safety_is_faulted()) {
        LOG_WRN("Safety fault: %s", safety_get_fault_reason());
        peltier_off();
        cooling_active = false;
        current_mode = MODE_OFF;
        return;
    }

    switch (phone_mode) {
    case MODE_OFF:
        if (cooling_active) {
            LOG_INF("Mode: off — stopping");
            peltier_off();
            cooling_active = false;
        }
        current_mode = MODE_OFF;
        break;

    case MODE_L1:
        if (!cooling_active) {
            LOG_INF("Mode: level 1 — start");
            safety_reset_timer();
            cooling_active = true;
        }
        peltier_set_level(MODE_L1);
        current_mode = MODE_L1;
        break;

    case MODE_L2:
        if (!cooling_active) {
            LOG_INF("Mode: level 2 — start");
            safety_reset_timer();
            cooling_active = true;
        }
        peltier_set_level(MODE_L2);
        current_mode = MODE_L2;
        break;

    case MODE_L3:
        if (!cooling_active) {
            LOG_INF("Mode: level 3 — start");
            safety_reset_timer();
            cooling_active = true;
        }
        peltier_set_level(MODE_L3);
        current_mode = MODE_L3;
        break;

    case MODE_AUTO: {
        int target = ble_phone_get_target_temp();
        auto_mode_set_target((float)target);
        bool should_cool = auto_mode_tick(vote.temperature);

        if (should_cool) {
            if (!cooling_active) {
                LOG_INF("Auto mode: cooling to %d°C (current: %.1f°C)",
                        target, vote.temperature);
                safety_reset_timer();
                cooling_active = true;
            }
            peltier_set_level(MODE_L3);
        } else {
            if (cooling_active) {
                LOG_INF("Auto mode: target reached — stopping");
                peltier_off();
                cooling_active = false;
            }
        }
        current_mode = MODE_AUTO;
        break;
    }

    default:
        peltier_off();
        cooling_active = false;
        current_mode = MODE_OFF;
        break;
    }
}

void main(void)
{
    int err;

    local_i2c = DEVICE_DT_GET(DT_NODELABEL(i2c1));
    if (!device_is_ready(local_i2c)) {
        LOG_ERR("Local I2C not ready");
        return;
    }

    err = bt_enable(NULL);
    if (err) {
        LOG_ERR("BLE init failed: %d", err);
        return;
    }

    temp_voter_init();
    peltier_init();
    safety_init();
    auto_mode_init();
    ble_patch_init();
    ble_phone_init();

    LOG_INF("Active Cooler started — 5 modes (Off/1/2/3/Auto)");

    while (1) {
        refresh_temp_sources();
        safety_tick();
        update_cooling();
        k_sleep(K_MSEC(CONTROL_INTERVAL_MS));
    }
}
