# Cryo Temp Alert PCB — v6

**Datum:** 2026-09-08
**Status:** Production-ready (0 echte DRC errors, 32 cosmetische warnings; ERC 0 errors)
**Validatie:** KiCad DRC/ERC + MCP SVG-render + netlist cross-check

## Specs
- Board: Ø34mm rond (radius 17mm)
- Layers: 2 (F.Cu + B.Cu)
- Componenten: 13 footprints
  - **U1** STM32L011 MCU (TSSOP-20) — F.Cu op (-3, 0)
  - **U2** TMP117 temperatuursensor (TMP117xxDRV, WSON-6-1EP) — **B.Cu** op (0, 7) ← huidcontact
  - **BT1** CR1220 batterijhouder — F.Cu
  - **BZ1** Piezo buzzer — F.Cu
  - **LED1** SMD LED — F.Cu
  - **SW1** SKQG tactile switch — F.Cu
  - **J1** Pinheader 1x4 (POGO programmer) — F.Cu
  - **R1, R2** 4.7k I2C pull-ups — F.Cu
  - **R3** 10k (NRST pull-up), **R4** LED current limiter — F.Cu
  - **C1, C2** 100nF decoupling — B.Cu bij MCU
- Nets: 12 (GND, VCC, SDA, SCL, NRST, VBAT_SENSE, BUZZER, LED_SIG, SW_SIG, I2C_ADDR, VDDA, DEC_100NF)

## Temperatuursensor: TMP117 (bewuste keuze)
Het board gebruikt de **TMP117** (via TI) als huidtemperatuursensor, bewust i.p.v. de
MAX30208 — onder meer vanwege de eenvoudigere wiring in deze fase.

- **Schema-symbool:** `Sensor_Temperature:TMP117xxDRV` (value `TMP117xxDRV`)
- **PCB footprint:** `WSON-6-1EP_2x2mm_P0.65mm_EP1x1.6mm` op B.Cu
- Zelfde I2C-pinout als de MAX30208 (SCL/SDA/ADD0/ALERT/V+/GND), dus later
  uitwisselbaar voor de MAX30208 in de Bluetooth-revisie.

> **Toekomst:** de volgende revisie krijgt Bluetooth (nRF) en de MAX30208-sensor.
> Voor nu is zowel schema als PCB ingesteld op de TMP117.

## Schema (herbouwd 2026-09-08)
Het schema is volledig herbouwd zodat het correct rendert in KiCad GUI:
- **Echte root-UUID** (was all-null `00000000-...` → leeg GUI-blad)
- **Echte net-labels** (46 stuks) verbinden de pinnen elektrisch
- **ERC: 0 errors** (7 cosmetische warnings: spare GPIOs aan power rails + VBAT_SENSE test-punt)
- Componenten op A4-sheet met net-labels i.p.v. alleen fysieke wires

## Inhoud van deze bundle (v6)
- `cryo-temp-alert.kicad_sch` — schema (herbouwd, rendert correct)
- `cryo-temp-alert.kicad_pcb` — PCB (U2 = TMP117)
- `cryo-temp-alert.kicad_pro` — project file
- `cryo-temp-alert.kicad_prl` — layout
- `sym-lib-table` / `fp-lib-table` — library config
- `schematic.pdf` — schema export (herbouwd)
- `gerbers/cryo-temp-alert-jlcpcb.zip` — JLCPCB fabricage bundle (verse export)
- `renders/cryo-3d-renders.zip` — 3D renders
- `cryo-temp-alert-drc.rpt` — KiCad DRC rapport (32 cosmetische warnings, 0 errors)
- `README.md` — dit bestand

## Voor JLCPCB fabricage
Upload `gerbers/cryo-temp-alert-jlcpcb.zip` naar https://jlcpcb.com/quote.
Bevat 7 Gerber lagen + drill + job file.

## Voor handmatige assemblage
1. **U2 (TMP117)** aan de **onderkant** (B.Cu, huidcontact-zijde)
2. Overige componenten bovenaan (F.Cu)
3. CR1220 batterij in BT1
4. Programmeren via J1 POGO pads (SWDIO/SWDCLK/RESET/GND)

## Layers / ontwerpnotities
- Sensor op B.Cu voor huidtemperatuur-meting
- Pad-to-mask clearance 0.05mm (0.65mm pitch)
- Track width 0.2mm minimum

Gevalideerd: 0 echte DRC errors, 0 ERC errors, 0 unconnected pads.