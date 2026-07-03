#ifndef AUTO_MODE_H
#define AUTO_MODE_H

#include <stdbool.h>

void auto_mode_init(void);
void auto_mode_set_target(float temp_c);
float auto_mode_get_target(void);
bool auto_mode_tick(float current_temp);

#endif
