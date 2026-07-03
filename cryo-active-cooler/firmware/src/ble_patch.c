#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ble_patch.h"

#define PATCH_NAME "CryoPatch"

static struct bt_conn *patch_conn = NULL;
static float patch_temp = 0.0f;
static bool has_temp = false;

static void device_found(const bt_addr_le_t *addr, int8_t rssi,
                         uint8_t type, struct net_buf_simple *ad)
{
    if (patch_conn != NULL) {
        return;
    }

    struct bt_data *data;
    struct net_buf_simple buf;

    net_buf_simple_init(&buf, ad->data);
    while (buf.len > 1) {
        uint8_t len = net_buf_simple_pull_u8(&buf);
        if (len == 0 || len > buf.len) break;
        uint8_t type = net_buf_simple_pull_u8(&buf);
        if (type == BT_DATA_NAME_SHORTENED || type == BT_DATA_NAME_COMPLETE) {
            if (buf.len >= sizeof(PATCH_NAME) - 1 &&
                memcmp(buf.data, PATCH_NAME, sizeof(PATCH_NAME) - 1) == 0) {
                bt_le_scan_stop();
                bt_conn_le_create(addr, BT_CONN_LE_CREATE_CONN,
                                  BT_LE_CONN_PARAM_DEFAULT, &patch_conn);
                return;
            }
        }
        net_buf_simple_pull(&buf, len - 1);
    }
}

void ble_patch_init(void)
{
    struct bt_le_scan_param scan_param = {
        .type = BT_LE_SCAN_TYPE_ACTIVE,
        .interval = 0x0060,
        .window = 0x0030,
    };
    bt_le_scan_start(&scan_param, device_found);
}

int ble_patch_get_temp(float *temp)
{
    if (!has_temp) {
        return -1;
    }
    *temp = patch_temp;
    return 0;
}

bool ble_patch_is_connected(void)
{
    return patch_conn != NULL;
}
