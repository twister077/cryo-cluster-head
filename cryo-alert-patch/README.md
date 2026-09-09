# Cryo Alert Patch

> Part of the [CryoClusterHead](../README.md) system — open-source cluster headache detection and relief.

**Temperature sensor patch for early detection of cluster headache flare-ups.**

This repo contains two hardware/firmware tracks:

---

## Track A — Non-BLE test patch (current, active)

A simplified, standalone test version of the patch with **no Bluetooth**. Used to validate the core hypothesis: *Can skin temperature monitoring reliably detect cluster headache prodromes?*

**Hardware:** STM32L011 MCU + TMP117 precision temperature sensor + piezo buzzer + LED + button (all on a 34mm round PCB).

**Firmware:** `firmware-stm32/` — PlatformIO + STM32Cube HAL.

| Feature | Status | Description |
|---------|--------|-------------|
| TMP117 I2C driver | ✅ | Reads skin temperature via sensor on B.Cu (direct skin contact) |
| ADC battery monitoring | ✅ | CR1220 voltage via VBAT_SENSE pin |
| dT/dt rate-of-change detection | ✅ | Rolling window, spike confirmed over consecutive high-rate samples |
| LED/buzzer alert state machine | ✅ | Normal / Warning / Critical / Muted states |
| Button debounce + snooze | ✅ | Press to silence alarm for 30 seconds |
| Low-power sleep | ✅ | Sleep mode (WFI) between measurements to extend battery life |
| Host-side unit tests | ✅ | 10/10 pass via `test/run_tests.sh` |
| BLE / phone connection | ⬜ | Not in scope for this track — see Track B |

### Quick start

```bash
cd firmware-stm32

# Run host-side unit tests (no hardware needed)
test/run_tests.sh

# Build firmware
pio run

# Flash via SWD (ST-Link / J-Link connected to J1 POGO pads)
pio run -t upload
```

See [`firmware-stm32/BUILD.md`](firmware-stm32/BUILD.md) for full build, flash, and pin mapping details.

---

## Track B — BLE version (planned, later phase)

The full Bluetooth Low Energy version with phone connectivity. This is the production target — the patch communicates with the [Cryo Mobile App](../cryo-mobile-app) for live dT/dt monitoring, push notifications, and data logging.

**Hardware:** nRF52832 MCU + MAX30205 clinical-grade temperature sensor.

**Firmware:** `firmware/` (Zephyr RTOS, Nordic SDK).

| Feature | Status | Description |
|---------|--------|-------------|
| BLE temperature broadcast | ⬜ | Advertise temperature via BLE advertising packets |
| MAX30205 sensor | ⬜ | Clinical-grade temperature sensing |
| Mobile app integration | ⬜ | Connect to Cryo Mobile App for live monitoring |
| Data logging | ⬜ | Store temperature history for later analysis |

> **Note:** The nRF52832 PCB design has known issues (pin mapping errors, missing crystal/antenna) that need to be resolved before Track B can be built. See the [project plan](../plan.md) for details.

---

## Components

| Component | Repository | Description |
|-----------|-----------|-------------|
| Alert Patch | `cryo-alert-patch` | This repo — firmware + hardware |
| Mobile App | `cryo-mobile-app` | Flutter + Rust Core — dT/dt detection |
| Active Cooler | `cryo-active-cooler` | Optional add-on — Peltier cooling module |

## License

- **Firmware:** MIT
- **Hardware:** CERN-OHL-S-2.0

> **⚠ Experimental device. Not a certified medical product. Use at your own risk.**
