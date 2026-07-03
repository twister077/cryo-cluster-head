#ifndef BLE_PHONE_H
#define BLE_PHONE_H

void ble_phone_init(void);
int ble_phone_get_temp(float *temp);
bool ble_phone_is_connected(void);
int ble_phone_get_mode(void);
int ble_phone_get_target_temp(void);

#endif
