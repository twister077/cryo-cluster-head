#ifndef BLE_PATCH_H
#define BLE_PATCH_H

void ble_patch_init(void);
int ble_patch_get_temp(float *temp);
bool ble_patch_is_connected(void);

#endif
