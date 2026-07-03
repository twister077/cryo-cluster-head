#ifndef PELTIER_H
#define PELTIER_H

void peltier_init(void);
void peltier_set_level(int level);
void peltier_off(void);
int peltier_get_level(void);

#endif
