# CryoClusterHead — Open-Source Cluster Headache Detection & Relief System

> **⚠ Experimental Research Project.** Not a certified medical product. Use at your own risk.

## Overview

Cluster headaches are among the most excruciatingly painful conditions known. This project explores an open-source hardware and software approach to **early detection of cluster headache flare-ups** using continuous skin temperature monitoring. 

By tracking rapid localized temperature changes ($\Delta T/\Delta t$) at the temple/neck area during the prodromal phase, our goal is to provide an early warning notification **minutes before peak pain hits**, allowing for timely intervention (such as cold therapy or medication).

---

## Current Focus & Development Tracks

The repository is structured into two main tracks:

### 🚀 Track A — Non-BLE Test Patch (Active Focus)
- **Hardware:** Miniature round PCB (`cryo-alert-patch/hardware/pcb/`) featuring **STM32L011** MCU + **TMP117** high-precision temperature sensor (B.Cu contact), CR1220 battery, piezo buzzer, LED, and tactile switch (`SW1`).
- **Firmware (`cryo-alert-patch/firmware-stm32/`)**: PlatformIO / STM32Cube HAL implementation featuring TMP117 I2C driver, ADC battery monitoring, alert state machine, and robust **$\Delta T/\Delta t$ flare-up detection** (validated with a host-side unit test suite).

### 📡 Track B — BLE & Companion Ecosystem (Planned / Later)
- **Hardware & Software:** nRF52832-based variants, `cryo-active-cooler/`, and `cryo-mobile-app/` (Flutter + Rust core).
- **Status:** Documented for future integration after Track A hardware testing and validation are completed.

---

## Community & Collaboration Welcome!

Building open-source medical/health hardware and embedded firmware is a massive undertaking for a single developer. **I cannot do this alone!** 

Whether you are an embedded firmware engineer, PCB designer (KiCad), mobile app developer, or a fellow cluster headache sufferer / researcher wanting to share insights — **your input, contributions, and feedback are immensely welcome.**

- **Open Issues & PRs:** Feel free to open issues or submit pull requests directly in this repository.
- **Discussions / Contact:** Reach out via GitHub issues or discussions to collaborate.

---

## Repository Structure

- `cryo-alert-patch/` — Hardware schematics, Gerber files, and firmware (`firmware-stm32/` for STM32 test track, `firmware-zephyr-ble/` for Zephyr/BLE).
- `cryo-mobile-app/` — Flutter companion app with Rust core for real-time $\Delta T/\Delta t$ analysis *(Track B)*.
- `cryo-active-cooler/` — Optional Peltier active cooling module concept *(Track B)*.
- `plan.md` — Detailed multi-phase project roadmap.

## License

- **Firmware:** MIT
- **Hardware:** CERN-OHL-S-2.0

---

## Contributors & AI Co-Developer

- **Twister077** — Project Initiator, Lead Concept & Hardware Architect.
- **Hermes Agent (Nous Research)** — AI Co-Developer & Engineering Assistant (assisted with KiCad schematic reconstruction, DRC/ERC validation, Gerber preparation, and STM32 PlatformIO firmware/testing).
