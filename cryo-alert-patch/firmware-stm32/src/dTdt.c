#include "dTdt.h"
#include <math.h>

#define SAMPLE_PERIOD_MS 2000
#define SPAN_MINUTES      ((float)SAMPLE_PERIOD_MS / 60000.0f)

void dTdt_Init(dTdt_t *s) {
    for (int i = 0; i < DTDT_WINDOW_SIZE; i++) s->buf[i] = 0;
    s->idx       = 0;
    s->count     = 0;
    s->spikeCount = 0;
    s->lastRate  = 0;
    s->lastTemp  = 0;
}

void dTdt_Push(dTdt_t *s, float tempC) {
    float prevTemp = s->lastTemp;

    s->buf[s->idx] = tempC;
    s->idx = (s->idx + 1) % DTDT_WINDOW_SIZE;
    if (s->count < DTDT_WINDOW_SIZE) s->count++;

    s->lastTemp = tempC;

    if (s->count < 2) {
        s->lastRate = 0;
        return;
    }

    float delta = tempC - prevTemp;
    float spanMin = SPAN_MINUTES;
    if (spanMin < 0.001f) spanMin = 0.001f;
    s->lastRate = delta / spanMin;
}

float dTdt_GetRate(dTdt_t *s) {
    return s->lastRate;
}

float dTdt_GetAvgTemp(dTdt_t *s) {
    if (s->count == 0) return 0;
    float sum = 0;
    for (int i = 0; i < s->count; i++) sum += s->buf[i];
    return sum / (float)s->count;
}

int dTdt_IsSpike(dTdt_t *s, float threshold) {
    float rate = fabsf(s->lastRate);
    if (rate >= threshold)
        s->spikeCount++;
    else
        s->spikeCount = 0;

    return (s->spikeCount >= DTDT_SPIKE_MIN_COUNT) ? 1 : 0;
}