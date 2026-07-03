# CryoClusterHead

**Early warning. Active relief. Open source.**

An open-source, modular system for **cluster headache and migraine detection and relief**. If you've ever tried calming your headache with gel masks from the freezer, cold showers, or smartphone coolers — you know the desperation. This project is about building something better together: A discreet temperature patch that detects flare-ups early, optionally paired with a discreet small Peltier cooler you can pull from your pocket and alerts sent straight to your phone.

## My Story — Why I Built This

I've been living with cluster headaches and migraines for years. My go-to relief has always been a gel headache mask straight from the freezer — that cold shock slows the nerves and temporarily constricts the blood vessels. Before that, I'd stick my head under a cold tap or jump into a pool if one was available in summer.

After buying a new phone for gaming, I picked up a GameSir FX5 Lite smartphone cooler. It worked incredibly well — according to AliExpress and YouTube reviewers, thanks to the Peltier element inside. The moment I saw its cooling capability, I thought about targeting specific spots on my head: temple, neck, forehead. During headache flare-ups it worked so well that I started sharing the idea and dreaming about an open-source, maker-built solution we could create together as a community. One temperature sensor the size of a diabetic patch, paired with a small Peltier cooling unit and smartphone alerts.

All the components already exist, the code is achievable, and there are plenty of motivated people — like you reading this — who want relief for themselves or for a much larger group. Sure, you can buy a smartphone cooler and hold it against your forehead with or without a headband (I'm getting a bit thin on top), but nobody is going to wear that outside. People with cluster headaches and migraines are already so limited socially.

Imagine if, like diabetics, we could just wear a discreet temperature patch that monitors flare-ups — without feeling like a cyborg (or maybe a full-on cyberpunk look, which would actually be cool). When a flare-up is detected, you grab a small device (30mm × 40mm) from your pocket, plug in a USB cable, and cool. Early detection, easy handling, and discreet. Peltier thermoelectric cooling has been used medically for brain flare-ups for years.

This project is my attempt to turn that vision into reality — and I hope you'll join me.

---

## Why This Exists

There are medical wearables for migraine (neuromodulation) and smart patches for baby fever, but **no consumer product** exists that combines active Peltier cooling with early thermal flare-up detection for cluster headache patients. Commercial players skip this space due to the small target audience and heavy certification costs.

CryoClusterHead fills that gap: a **DIY, maker-friendly** system you can build yourself — no medical certification required.

---

## How It Works

```
┌─────────────────┐   BLE    ┌──────────────────┐   BLE    ┌──────────────────┐
│  Alert Patch    │─────────▶│   Mobile App     │─────────▶│  Active Cooler   │
│  (nRF52         │          │  (Flutter + Rust) │          │  (nRF52 +        │
│   MAX30205)     │          │                   │          │   Peltier)       │
│  24/7 on temple │          │  ┌─────────────┐  │          │  Optional add-on │
│                 │          │  │ dT/dt algo  │  │          │  on headband     │
└─────────────────┘          │  │ (Rust core) │  │          └──────────────────┘
                             │  └─────────────┘  │
                             │  ┌─────────────┐  │
                             │  │ Push alert  │  │
                             │  │ + history   │  │
                             │  └─────────────┘  │
                             └──────────────────┘
```

1. **Wear the Alert Patch** on your temple or neck — it monitors skin temperature 24/7 via a clinical-grade MAX30205 sensor
2. **Mobile App detects flare-ups** — the dT/dt algorithm catches rapid temperature rises **before peak pain**
3. **Get alerted** — push notification gives you time to prepare or take medication
4. **Optional: Active Cooler** — if equipped, auto-cools at 3 levels based on flare-up severity

---

## The Innovation: dT/dt Detection

Traditional fever patches use **static thresholds** (e.g. `>38.5°C`). Cluster headache attacks involve a **rapid temperature rise** — the derivative (rate of change) matters more than the absolute temperature.

CryoClusterHead's mobile app runs a **dT/dt algorithm** in its Rust core engine:
- Computes the slope of temperature over time using linear regression over a sliding window
- Configurable threshold and window size
- Debounce logic prevents false positives
- Alerts during the **prodromal phase** — minutes before peak pain

All processing happens **on-device**. No cloud, no data leaving your phone.

---

## Philosophy & Design Principles

No unnecessary buttons, screens, or complex menus on your head during an attack. The system is split into two independent modules — a featherweight sensor for 24/7 detection and a powerful, "dumb" cooler for direct action.

### Power in Your Pocket
- The Active Cooler has **no internal battery** — it runs from a standard USB-C PD power bank in your pocket
- Eliminates weight on the head (<150g total)
- Eliminates the risk of unprotected LiPo cells near your face
- Unplug the cable = instant hardware kill

### Safety First
- **Triple-redundant temperature voting** — 3 independent sources must agree
- **15-minute hard cutoff** — regardless of cooling level
- **Safe shutdown** if fewer than 2 temperature sources agree
- **Hardware kill** — unplugging USB instantly stops cooling

### Modular by Design
- **Alert Patch + Mobile App** is a complete standalone system — useful without the cooler
- **Active Cooler** is an optional add-on for those who want active relief
- All sub-projects are independent and can be built separately

### No Cloud
All BLE parsing, dT/dt computation, and notification logic runs on-device (Rust core → Flutter UI). Your temperature data stays yours.

---

## Who Is This For?

| Audience | What you get |
|----------|-------------|
| **Cluster headache patients** | A system that gives you early warning and optional active cooling — build it or find someone to build it for you |
| **Makers / DIY enthusiasts** | Complete open-source hardware + firmware + software — solder, flash, and run |
| **Contributors** | Rust core, Flutter app, Zephyr firmware, KiCad hardware — all open for improvements |

---

## Components

| Component | MCU | Sensor / Actuator | Power | BOM Cost | Build Time | Status |
|-----------|-----|-------------------|-------|:--------:|:--------:|:------:|
| **Alert Patch** | nRF52832 (QFN-48) | MAX30205 (clinical temp sensor) | CR1220 coin cell (~10,000h) | ~$18 | 1-2h | Prototype |
| **Mobile App** | — | dT/dt algo in Rust core | Phone battery | — | `cargo build` | v0.1 |
| **Active Cooler** | nRF52832 (QFN-48) | Peltier TES1-04903 + 30mm fan + CH224K USB-PD trigger | USB-C PD 9/12V power bank | ~$41 | 2-3h | Prototype |

### Alert Patch
- Coin-sized (20×20mm PCB), TPU 3D-printed enclosure
- Worn on temple or neck with standard CGM adhesive patches
- BLE advertisement every 60s — deep sleep between reads
- Broadcasts temperature (°C × 100), battery (mV), sequence counter

### Mobile App (Flutter + Rust)
- **Rust core:** BLE parser + dT/dt detector + notification logic — exposed via C FFI
- **Flutter UI:** Live temperature dashboard, 24h history with charts, settings
- Controls the Active Cooler via BLE (mode + target temperature)

### Active Cooler
- Headband-mounted (PETG 3D-print + elastic band)
- 3 cooling levels (33% / 66% / 100% PWM) + auto mode
- Triple temperature voting:

```
┌──────────────────────┐
│ Patch BLE            │──┐
│ (via phone relay)    │  │
└──────────────────────┘  │
                          ├──► Firmware majority vote
┌──────────────────────┐  │    (≥2 agree → use that temp)
│ Patch BLE            │──┘
│ (direct)             │
└──────────────────────┘
┌──────────────────────┐
│ Local sensor         │
│ (MAX30205 on         │
│  cold side)          │
└──────────────────────┘
```

If fewer than 2 sources agree → **safe shutdown** (Level 0, Peltier off).

---

## Safety

| Feature | Mechanism |
|---------|-----------|
| Triple temp voting | 3 independent sources, ≥2 must agree |
| 15-min hard cutoff | Firmware timer, regardless of level |
| Auto step-down | Gradual reduction as temperature normalizes |
| Hardware kill | Unplug USB = Peltier instantly off |
| No head battery | All power in pocket via USB-PD |

---

## Quick Start

### Alert Patch (Zephyr firmware)

```bash
cd cryo-alert-patch/firmware
west init -l .
west update
west build -b nrf52832dk_nrf52832 .
west flash
```

### Mobile App

```bash
# Rust core
cd cryo-mobile-app/rust-core
cargo test
cargo build --release

# Flutter app
cd cryo-mobile-app/flutter
flutter pub get
flutter run
```

### Active Cooler (Zephyr firmware)

```bash
cd cryo-active-cooler/firmware
west init -l .
west update
west build -b nrf52832dk_nrf52832 .
west flash
```

---

## Tech Stack

| Component | Language / Framework | Platform |
|-----------|---------------------|----------|
| Alert Patch firmware | C (Zephyr RTOS) | nRF52832 |
| Active Cooler firmware | C (Zephyr RTOS) | nRF52832 |
| Rust core engine | Rust | Android / iOS via FFI |
| Mobile UI | Dart (Flutter) | Android / iOS |
| Hardware | KiCad-style PCB, 3D-print (TPU / PETG) | — |

---

## License

- **Firmware & Software:** MIT
- **Hardware designs:** CERN Open Hardware Licence Version 2 — Strongly Reciprocal (CERN-OHL-S-2.0)

---

> **⚠ Experimental device. Not a certified medical product. Use at your own risk.**
> Incorrect configuration or unskilled use of Peltier elements can cause burns or frostbite.
> Only build and use this if you are familiar with electronics safety, battery management, and thermal control.
