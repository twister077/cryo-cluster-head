#include "dTdt.h"
#include <math.h>

/* Defined in main.c — sample period in milliseconds */
#define SAMPLE_PERIOD_MS  2000

void dTdt_Init(dTdt_t *s) {
    for (int i = 0; i < DTDT_WINDOW_SIZE; i++) s->buf[i] = 0;
    s->idx      = 0;
    s->count    = 0;
    s->lastRate = 0;
    s->lastTemp = 0;
}

/* Push a new temperature reading into the ring buffer */
void dTdt_Push(dTdt_t *s, float tempC) {
    s->lastTemp        = tempC;
    s->buf[s->idx]     = tempC;
    s->idx             = (s->idx + 1) % DTDT_WINDOW_SIZE;
    if (s->count < DTDT_WINDOW_SIZE) s->count++;

    if (s->count < 2) {
        s->lastRate = 0;
        return;
    }
    /* oldest sample is at s->idx when buffer is full, otherwise at 0 */
    uint8_t oldest_idx;
    if (s->count == DTDT_WINDOW_SIZE)
        oldest_idx = s->idx;
    else
        oldest_idx = 0;

    float delta = s->lastTemp - s->buf[oldest_idx];
    float spanMin = ((float)(s->count - 1) * (float)SAMPLE_PERIOD_MS) / 60000.0f;
    if (spanMin < 0.001f) spanMin = 0.001f;
    s->lastRate = delta / spanMin;   /* °C per minute */
}

float dTdt_GetRate(dTdt_t *s) {
    return s->lastRate;
}

float dTdt_GetAvgTemp(dTdt_t *s) {
    if (s->count == 0) return 0;
    float sum = 0;
    uint8_t n  = s->count;
    for (int i = 0; i < n; i++) sum += s->buf[i];
    return sum / (float)n;
}

int dTdt_IsSpike(dTdt_t *s, float threshold) {
    /* Spike = absolute rate above threshold on latest reading
       AND the previous reading also exceeded threshold (2 consecutive) */
    if (s->count < 2) return 0;
    float rate = fabsf(s->lastRate);
    if (rate < threshold) return 0;

    /* Recompute rate at previous index to check consecutiveness */
    if (s->count < 3) return (rate >= threshold) ? 1 : 0;

    uint8_t prev_idx = (s->idx == 0) ? (DTDT_WINDOW_SIZE - 1) : (s->idx - 1);
    uint8_t prev_prev = (prev_idx == 0) ? (DTDT_WINDOW_SIZE - 1) : (prev_idx - 1);
    float prevRate = fabsf(s->buf[prev_idx] - s->buf[prev_prev]);
    float spanMin  = ((float)1 * (float)SAMPLE_PERIOD_MS) / 60000.0f;
    if (spanMin < 0.001f) spanMin = 0.001f;
    prevRate /= spanMin;
    return (prevRate >= threshold) ? 1 : 0;
}
