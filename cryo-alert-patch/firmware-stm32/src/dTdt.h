#ifndef DTDT_H
#define DTDT_H

#include <stdint.h>

#define DTDT_WINDOW_SIZE 10  /* rolling window of samples */

typedef struct {
    float buf[DTDT_WINDOW_SIZE];
    uint8_t idx;
    uint8_t count;       /* how many valid samples (up to DTDT_WINDOW_SIZE) */
    float  lastRate;     /* most recent dT/dt in °C/min */
    float  lastTemp;     /* most recent raw temperature */
} dTdt_t;

void  dTdt_Init(dTdt_t *s);
void  dTdt_Push(dTdt_t *s, float tempC);
float dTdt_GetRate(dTdt_t *s);      /* °C per minute */
float dTdt_GetAvgTemp(dTdt_t *s);   /* moving average */
int   dTdt_IsSpike(dTdt_t *s, float threshold); /* rate > threshold for >=2 consecutive reads */

#endif
