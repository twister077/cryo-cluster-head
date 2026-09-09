# Cryo Alert Patch — Assembly & Usage (STM32 Non-BLE Track)

This guide is for the **current STM32L011-based non-Bluetooth test patch** (Track A).

## PCB Assembly & Flashing (Track A - STM32L011)

This track uses the STM32L011 microcontroller and the TMP117 temperature sensor. It does **NOT** use Bluetooth or the nRF52832.

1.  **Hardware:** Refer to `cryo-alert-patch/hardware/pcb/` for KiCad files, Gerbers, and schematics.
2.  **Firmware:** The source code is located in `cryo-alert-patch/firmware-stm32/`.

### Flashing Instructions (STM32L011 via SWD)

Connect a ST-Link debugger or similar SWD programmer to the **J1 POGO pin header** (Pinout: SWDIO, SWDCLK, RESET, GND).

Then, use **PlatformIO** to build and flash the firmware:

```bash
# Navigate to the firmware directory
cd cryo-alert-patch/firmware-stm32

# Build the firmware
pio run

# Flash the firmware to the board
pio run --target upload
```

## Future / BLE Track (Track B - nRF52832)

This track is **planned for a later revision** and will involve the nRF52832 microcontroller and BLE functionality. The current PCB design for Track A is not compatible with BLE components.

*   **Hardware:** See `cryo-alert-patch/hardware/pcb/README.md` for notes on the nRF52832 issues.
*   **Firmware:** Located in `cryo-alert-patch/firmware/` (Zephyr RTOS based).

