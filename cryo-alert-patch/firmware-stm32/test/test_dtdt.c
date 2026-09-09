/* Host-side unit test for the dTdt flare-up detection algorithm.
 * Compiles the real src/dTdt.c directly against the real header.
 * Mirrors the main-loop pattern: per measurement cycle, one Push then one IsSpike.
 *
 * Build & run:
 *   cd cryo-alert-patch/firmware-stm32
 *   gcc -I src -o test/test_dtdt test/test_dtdt.c src/dTdt.c -lm
 *   ./test/test_dtdt
 */

#include <stdio.h>
#include <math.h>
#include "dTdt.h"

static int failures = 0;
static int checks = 0;

static void check_close(const char *name, float got, float want, float tol) {
    checks++;
    if (fabsf(got - want) > tol) {
        failures++;
        printf("FAIL %s: got %.4f, want %.4f\n", name, got, want);
    }
}

static void check_int(const char *name, int got, int want) {
    checks++;
    if (got != want) {
        failures++;
        printf("FAIL %s: got %d, want %d\n", name, got, want);
    }
}

/* Feed N consecutive samples, calling IsSpike after each (as main loop does). */
static int feed(dTdt_t *s, const float *temps, int n, float threshold) {
    int spike = 0;
    for (int i = 0; i < n; i++) {
        dTdt_Push(s, temps[i]);
        if (dTdt_IsSpike(s, threshold)) spike = 1;
    }
    return spike;
}

int main(void) {
    dTdt_t s;

    /* --- Init --- */
    dTdt_Init(&s);
    check_int("init:count=0", s.count, 0);
    check_int("init:spikeCount=0", s.spikeCount, 0);
    check_close("init:rate=0", dTdt_GetRate(&s), 0.0f, 1e-6f);
    check_close("init:avg=0", dTdt_GetAvgTemp(&s), 0.0f, 1e-6f);

    /* --- Steady temperature: no rate, no spike --- */
    dTdt_Init(&s);
    float steady[5] = {36.5f, 36.5f, 36.5f, 36.5f, 36.5f};
    check_int("steady:no-spike", feed(&s, steady, 5, 0.5f), 0);
    check_close("steady:avg=36.5", dTdt_GetAvgTemp(&s), 36.5f, 1e-4f);

    /* --- Rapid rise: 4 samples climbing (delta +0.5°C/2s = 15°C/min each).
          3 consecutive high rates -> spike fires. --- */
    dTdt_Init(&s);
    float rise[4] = {36.0f, 36.5f, 37.0f, 37.5f};
    check_int("rapid-rise:spike", feed(&s, rise, 4, 0.5f), 1);

    /* --- Rapid drop (cold): should trigger too --- */
    dTdt_Init(&s);
    float drop[4] = {37.5f, 37.0f, 36.5f, 36.0f};
    check_int("rapid-drop:spike-neg", feed(&s, drop, 4, 0.5f), 1);

    /* --- Slow drift below threshold: must NOT trigger.
          +0.01°C / 2s = 0.3°C/min < 0.5 threshold --- */
    dTdt_Init(&s);
    int slowspike = 0;
    for (int i = 0; i < 20; i++) {
        dTdt_Push(&s, 36.0f + 0.01f * i);
        if (dTdt_IsSpike(&s, 0.5f)) slowspike = 1;
    }
    check_int("slow:no-spike", slowspike, 0);

    /* --- Single blip (1 high rate only): must NOT trigger. --- */
    dTdt_Init(&s);
    float blip[5] = {36.5f, 36.5f, 38.0f, 36.5f, 36.5f};
    check_int("blip:no-spike", feed(&s, blip, 5, 0.5f), 0);

    printf("\n%d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}