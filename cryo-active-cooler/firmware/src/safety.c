#include <zephyr/kernel.h>
#include "safety.h"
#include "levels.h"

#define RUN_TIMER_MS (MAX_CYCLE_MIN * 60 * 1000)

static bool faulted = false;
static bool timer_exp = false;
static int64_t start_time = 0;
static const char *fault_reason = "none";

void safety_init(void)
{
    faulted = false;
    timer_exp = false;
    start_time = 0;
    fault_reason = "none";
}

void safety_tick(void)
{
    if (start_time == 0) {
        return;
    }

    int64_t elapsed = k_uptime_get() - start_time;
    if (elapsed >= RUN_TIMER_MS) {
        timer_exp = true;
        faulted = true;
        fault_reason = "max runtime exceeded";
    }
}

bool safety_is_faulted(void)
{
    return faulted;
}

bool safety_timer_expired(void)
{
    return timer_exp;
}

void safety_reset_timer(void)
{
    start_time = k_uptime_get();
    timer_exp = false;
}

const char *safety_get_fault_reason(void)
{
    return fault_reason;
}
