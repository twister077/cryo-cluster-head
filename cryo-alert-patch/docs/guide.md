# Alert Patch — Assembly & Usage

## PCB Assembly
1. Reflow nRF52832 (QFN-48) — verify all pins.
2. Solder MAX30205 (TDFN-8) — observe orientation.
3. Solder passives (C1-C3, R1-R2) and battery holder.
4. Inspect for shorts under microscope.

## Flashing
```bash
west build -b nrf52832dk_nrf52832 .
west flash
```

## BLE Advertisement Format
| Byte | Field | Description |
|------|-------|-------------|
| 0-1 | Manufacturer ID | 0x0059 (Nordic) |
| 2-3 | Temperature | °C × 100 |
| 4 | Battery | mV / 10 |
| 5 | Sequence | 0-255 counter |
| 6 | Status | Bitfield |

## Power Profile
- Active: ~5 mA for 3 ms every 60 s
- Deep sleep: ~1 µA
- Average: ~4 µA → ~10,000 hours on CR1220
