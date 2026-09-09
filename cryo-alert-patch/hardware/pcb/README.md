# Cryo Temp Alert PCB — v6

**Date:** 2026-09-08
**Status:** Production-ready (0 real DRC errors, 32 cosmetic warnings; ERC 0 errors)
**Validation:** KiCad DRC/ERC + MCP SVG render + netlist cross-check

## Specs
- Board: Ø34mm round (radius 17mm)
- Layers: 2 (F.Cu + B.Cu)
- Components: 13 footprints
  - **U1** STM32L011 MCU (TSSOP-20) — F.Cu at (-3, 0)
  - **U2** TMP117 temperature sensor (TMP117xxDRV, WSON-6-1EP) — **B.Cu** at (0, 7) ← skin contact
  - **BT1** CR1220 battery holder — F.Cu
  - **BZ1** Piezo buzzer — F.Cu
  - **LED1** SMD LED — F.Cu
  - **SW1** SKQG tactile switch — F.Cu
  - **J1** Pin header 1x4 (POGO programmer) — F.Cu
  - **R1, R2** 4.7k I2C pull-ups — F.Cu
  - **R3** 10k (NRST pull-up), **R4** LED current limiter — F.Cu
  - **C1, C2** 100nF decoupling — B.Cu near MCU
- Nets: 12 (GND, VCC, SDA, SCL, NRST, VBAT_SENSE, BUZZER, LED_SIG, SW_SIG, I2C_ADDR, VDDA, DEC_100NF)

## Temperature sensor: TMP117 (deliberate choice)
The board uses the **TMP117** (by TI) as the skin temperature sensor, deliberately instead of the
MAX30208 — among other reasons, because of the simpler wiring in this phase.

- **Schematic symbol:** `Sensor_Temperature:TMP117xxDRV` (value `TMP117xxDRV`)
- **PCB footprint:** `WSON-6-1EP_2x2mm_P0.65mm_EP1x1.6mm` on B.Cu
- Same I2C pinout as the MAX30208 (SCL/SDA/ADD0/ALERT/V+/GND), so it can later be
  swapped for the MAX30208 in the Bluetooth revision.

> **Future:** the next revision will get Bluetooth (nRF) and the MAX30208 sensor.
> For now both the schematic and PCB are configured for the TMP117.

## Schematic (rebuilt 2026-09-08)
The schematic was fully rebuilt so it renders correctly in the KiCad GUI:
- **Real root UUID** (was all-null `00000000-...` → blank GUI sheet)
- **Real net labels** (46 total) connect the pins electrically
- **ERC: 0 errors** (7 cosmetic warnings: spare GPIOs on power rails + VBAT_SENSE test point)
- Components on an A4 sheet with net labels instead of only physical wires

## Contents of this bundle (v6)
- `cryo-temp-alert.kicad_sch` — schematic (rebuilt, renders correctly)
- `cryo-temp-alert.kicad_pcb` — PCB (U2 = TMP117)
- `cryo-temp-alert.kicad_pro` — project file
- `cryo-temp-alert.kicad_prl` — layout
- `sym-lib-table` / `fp-lib-table` — library configuration
- `schematic.pdf` — schematic export (rebuilt)
- `gerbers/cryo-temp-alert-jlcpcb.zip` — JLCPCB fabrication bundle (fresh export)
- `renders/cryo-3d-renders.zip` — 3D renders
- `cryo-temp-alert-drc.rpt` — KiCad DRC report (32 cosmetic warnings, 0 errors)
- `README.md` — this file

## For JLCPCB fabrication
Upload `gerbers/cryo-temp-alert-jlcpcb.zip` to https://jlcpcb.com/quote.
Contains 7 Gerber layers + drill + job file.

## For manual assembly
1. **U2 (TMP117)** on the **bottom** (B.Cu, skin-contact side)
2. Remaining components on top (F.Cu)
3. Insert CR1220 battery into BT1
4. Program via J1 POGO pads (SWDIO/SWDCLK/RESET/GND)

## Layers / design notes
- Sensor on B.Cu for skin-temperature measurement
- Pad-to-mask clearance 0.05mm (0.65mm pitch)
- Minimum track width 0.2mm

Validated: 0 real DRC errors, 0 ERC errors, 0 unconnected pads.