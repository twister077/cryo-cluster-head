# Cryo Alert Patch

> Part of the [CryoClusterHead](../README.md) system — open-source cluster headache detection and relief.

**BLE temperature sensor patch for early detection of cluster headache flare-ups.**

A lightweight, always-on temperature patch that monitors skin temperature via a MAX30205 clinical-grade sensor and broadcasts readings via BLE. When used with the [Cryo Mobile App](../cryo-mobile-app), it detects rapid temperature spikes (dT/dt) characteristic of cluster headache attacks and sends a push notification — **before the peak pain hits.**

## Standalone Value

The Alert Patch + Mobile App is a **complete standalone system.** You don't need the Active Cooler to benefit:

- **24/7 monitoring** — wear the patch on your temple or neck
- **Flare-up detection** — alert on rapid temperature rise, not static thresholds
- **Early warning** — get notified during the prodromal phase, minutes before peak pain
- **Data logging** — track temperature patterns over time to identify triggers

## Components

| Component | Repository | Description |
|-----------|-----------|-------------|
| Alert Patch | `cryo-alert-patch` | This repo — firmware + hardware |
| Mobile App | `cryo-mobile-app` | Flutter + Rust Core — dT/dt detection |
| Active Cooler | `cryo-active-cooler` | Optional add-on — Peltier cooling module |

## Quick Start

```bash
# Build firmware
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
