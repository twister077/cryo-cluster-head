#ifndef SAFETY_H
#define SAFETY_H

#include <stdbool.h>

void safety_init(void);
void safety_tick(void);
bool safety_is_faulted(void);
bool safety_timer_expired(void);
void safety_reset_timer(void);
const char *safety_get_fault_reason(void);

#endif
