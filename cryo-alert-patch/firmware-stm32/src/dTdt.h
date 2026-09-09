#ifndef DTDT_H
#define DTDT_H

#include <stdint.h>

#define DTDT_WINDOW_SIZE 10
#define DTDT_SPIKE_MIN_COUNT 3  /* need at least this many high-rate samples in a row */

typedef struct {
    float buf[DTDT_WINDOW_SIZE];
    uint8_t idx;
    uint8_t count;
    uint8_t spikeCount;   /* consecutive samples with rate above threshold */
    float lastRate;       /* most recent dT/dt in °C/min */
    float lastTemp;       /* most recent raw temperature */
} dTdt_t;

void  dTdt_Init(dTdt_t *s);
void  dTdt_Push(dTdt_t *s, float tempC); /* push + recompute rate */
float dTdt_GetRate(dTdt_t *s);
float dTdt_GetAvgTemp(dTdt_t *s);
int   dTdt_IsSpike(dTdt_t *s, float threshold);

#endif
