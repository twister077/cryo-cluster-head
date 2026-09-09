# Cryo Temp Alert v6 — Build & Flash Instructions

## Prerequisites

- **PlatformIO CLI** (`pio`)
- **ST-Link** or **J-Link** debugger/programmer
- **SWD header** — POGO pads (J1) on the PCB: SWDIO, SWDCLK, RESET, GND

## Build

```bash
cd cryo-alert-patch/firmware-stm32
pio run
```

This compiles the firmware for the `nucleo_l011k4` target (STM32L011K4T6).
Output: `.pio/build/nucleo_l011k4/firmware.bin`

## Flash via ST-Link (SWD)

Connect your ST-Link (or Nucleo board) to the POGO pads on the PCB:

| POGO Pad | STM32 Pin | Debugger Wire |
|----------|-----------|---------------|
| GND      | GND       | GND           |
| SWDIO    | PA13      | SWDIO         |
| SWDCLK   | PA14      | SWDCLK        |
| RESET    | NRST      | NRST          |

```bash
# Flash + reset
pio run --target upload

# Monitor serial output (if UART is connected)
pio device monitor
```

## Build & Run Tests (Host-side)

The dT/dt detection algorithm has host-side unit tests that run on your PC
(without hardware):

```bash
cd cryo-alert-patch/firmware-stm32
gcc -I src -o test/test_dtdt test/test_dtdt.c src/dTdt.c -lm
./test/test_dtdt
```

Expected output: `10 checks, 0 failures`

## Resource Usage

| Resource | Used | Total | Util |
|----------|------|-------|------|
| RAM      | 300 B | 2048 B | 14.6% |
| Flash    | 11 KB | 16 KB | 67.5% |
