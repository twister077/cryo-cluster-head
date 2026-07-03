#include <zephyr/kernel.h>
#include "auto_mode.h"
#include "levels.h"

static float target_temp = AUTO_TARGET_DEFAULT;
static float current_temp = 0.0f;
static bool peltier_on = false;

void auto_mode_init(void)
{
    target_temp = AUTO_TARGET_DEFAULT;
    current_temp = 0.0f;
    peltier_on = false;
}

void auto_mode_set_target(float temp_c)
{
    if (temp_c < AUTO_TARGET_MIN) {
        temp_c = AUTO_TARGET_MIN;
    }
    if (temp_c > AUTO_TARGET_MAX) {
        temp_c = AUTO_TARGET_MAX;
    }
    target_temp = temp_c;
}

float auto_mode_get_target(void)
{
    return target_temp;
}

bool auto_mode_tick(float current)
{
    current_temp = current;

    if (current_temp > target_temp + AUTO_HYSTERESIS) {
        peltier_on = true;
    } else if (current_temp < target_temp - AUTO_HYSTERESIS) {
        peltier_on = false;
    }

    return peltier_on;
}
