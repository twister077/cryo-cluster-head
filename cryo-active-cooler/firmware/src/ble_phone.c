#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>

#include "ble_phone.h"
#include "levels.h"

static struct bt_conn *phone_conn = NULL;
static float phone_temp = 0.0f;
static bool has_temp = false;

static int cooler_mode = MODE_OFF;
static int target_temp = AUTO_TARGET_DEFAULT;

static ssize_t on_write_mode(struct bt_conn *conn,
                              const struct bt_gatt_attr *attr,
                              const void *buf, uint16_t len,
                              uint16_t offset, uint8_t flags)
{
    if (offset != 0 || len != 1) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }
    uint8_t val = *((const uint8_t *)buf);
    if (val <= MODE_AUTO) {
        cooler_mode = val;
    }
    return len;
}

static ssize_t on_write_target(struct bt_conn *conn,
                                const struct bt_gatt_attr *attr,
                                const void *buf, uint16_t len,
                                uint16_t offset, uint8_t flags)
{
    if (offset != 0 || len != 1) {
        return BT_GATT_ERR(BT_ATT_ERR_INVALID_OFFSET);
    }
    uint8_t val = *((const uint8_t *)buf);
    if (val >= AUTO_TARGET_MIN && val <= AUTO_TARGET_MAX) {
        target_temp = val;
    }
    return len;
}

BT_GATT_SERVICE_DEFINE(cooler_ctrl_svc,
    BT_GATT_PRIMARY_SERVICE(BT_UUID_DECLARE_128(
        BT_UUID_128_ENCODE(0x0000cry0, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb))),

    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(
        BT_UUID_128_ENCODE(0x0000cry1, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)),
        BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_WRITE,
        NULL, on_write_mode, NULL),

    BT_GATT_CHARACTERISTIC(BT_UUID_DECLARE_128(
        BT_UUID_128_ENCODE(0x0000cry2, 0x0000, 0x1000, 0x8000, 0x00805f9b34fb)),
        BT_GATT_CHRC_WRITE,
        BT_GATT_PERM_WRITE,
        NULL, on_write_target, NULL),
);

void ble_phone_init(void)
{
    const struct bt_data ad[] = {
        BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR),
        BT_DATA(BT_DATA_NAME_COMPLETE, "CryoCooler", sizeof("CryoCooler") - 1),
    };
    bt_le_adv_start(BT_LE_ADV_CONN, ad, ARRAY_SIZE(ad), NULL, 0);
}

int ble_phone_get_temp(float *temp)
{
    if (!has_temp) {
        return -1;
    }
    *temp = phone_temp;
    return 0;
}

bool ble_phone_is_connected(void)
{
    return phone_conn != NULL;
}

int ble_phone_get_mode(void)
{
    return cooler_mode;
}

int ble_phone_get_target_temp(void)
{
    return target_temp;
}
