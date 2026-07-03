# Cryo Active Cooler

> Part of the [CryoClusterHead](../README.md) system — open-source cluster headache detection and relief.

**Smart Peltier cooling module with triple-redundant temperature safety.**

An optional add-on for the CryoClusterHead system. When a flare-up is detected, this headband-mounted module provides active cooling directly to the painful area.

## Features

Unlike simple Peltier drivers, the Active Cooler is a safety-first intelligent module:

- **3 cooling levels** — automatically selected based on flare-up severity
- **Triple temp voting** — reads temperature from 3 independent sources for fail-safe operation
- **15-minute max timer** — hard limit prevents over-cooling
- **Auto step-down** — gradually reduces cooling as temperature normalizes
- **No phone required** — can read the Alert Patch directly via BLE

## Temperature Safety Voting

```
┌──────────────────┐
│ Patch BLE        │──┐
│ (via phone relay)│  │
└──────────────────┘  │
                      ├──► Firmware majority vote
┌──────────────────┐  │    (≥2 agree → use that temp)
│ Patch BLE        │──┘
│ (direct)         │
└──────────────────┘
┌──────────────────┐
│ Local sensor     │
│ (MAX30205 on     │
│  cold side)      │
└──────────────────┘
```

If fewer than 2 sources agree → **safe shutdown** (Level 0).

## Cooling Levels

| Level | Condition | PWM | Description |
|-------|-----------|-----|-------------|
| 0 | Normal temp / safety fault | 0% | Off |
| 1 | Mild rise (+0.3–0.8°C) | 33% | Gentle cooling |
| 2 | Moderate rise (+0.8–1.5°C) | 66% | Active cooling |
| 3 | Strong rise (>1.5°C) | 100% | Max cooling |

- Each level auto-advances based on current temperature
- Auto step-down as temp drops
- Hard cutoff after 15 minutes regardless of level

## Quick Start

```bash
cd firmware
west init -l .
west update
west build -b nrf52832dk_nrf52832 .
west flash
```

## License

- **Firmware:** MIT
- **Hardware:** CERN-OHL-S-2.0

> **⚠ Experimental device. Not a certified medical product. Use at your own risk.**
