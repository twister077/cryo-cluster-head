#include <zephyr/kernel.h>
#include <zephyr/drivers/pwm.h>
#include "peltier.h"
#include "levels.h"

static const struct device *pwm_dev;
static int current_level = LEVEL_0_OFF;

#define PWM_NODE DT_ALIAS(pwm_peltier)
#define PWM_PERIOD PWM_USEC(20000)

void peltier_init(void)
{
    pwm_dev = DEVICE_DT_GET(PWM_NODE);
    if (!device_is_ready(pwm_dev)) {
        printk("PWM device not ready\n");
        return;
    }
    peltier_off();
}

void peltier_set_level(int level)
{
    uint32_t duty = 0;

    switch (level) {
    case LEVEL_0_OFF:
        duty = 0;
        break;
    case LEVEL_1_MILD:
        duty = PWM_PERIOD * PWM_LEVEL_1 / 100;
        break;
    case LEVEL_2_ACTIVE:
        duty = PWM_PERIOD * PWM_LEVEL_2 / 100;
        break;
    case LEVEL_3_MAX:
        duty = PWM_PERIOD * PWM_LEVEL_3 / 100;
        break;
    default:
        duty = 0;
        break;
    }

    pwm_set(pwm_dev, 0, PWM_PERIOD, duty);
    current_level = level;
}

void peltier_off(void)
{
    peltier_set_level(LEVEL_0_OFF);
}

int peltier_get_level(void)
{
    return current_level;
}
