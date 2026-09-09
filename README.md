# CryoClusterHead — Open-Source Cluster Headache Detection & Relief System

> **⚠ Experimental Research Project.** Not a certified medical product. Use at your own risk.

## Overview

Cluster headaches are among the most excruciatingly painful conditions known. This project explores an open-source hardware and software approach to **early detection of cluster headache flare-ups** using continuous skin temperature monitoring. 

By tracking rapid localized temperature changes ($\Delta T/\Delta t$) at the temple/neck area during the prodromal phase, our goal is to provide an early warning notification **minutes before peak pain hits**, allowing for timely intervention (such as cold therapy or medication).

---

## Current Focus: The Cryo Temp Alert Patch (v6)

We have just completed the production-ready v6 hardware files for a miniature wearable test PCB (`cryo-alert-patch`), optimized for JLCPCB fabrication:
- **Form Factor:** Ø34mm round 2-layer PCB designed for temple/skin placement.
- **Sensor:** TMP117 high-precision temperature sensor positioned on the bottom layer (B.Cu) for direct skin contact.
- **Microcontroller & Comm:** Designed for low-power operation and BLE connectivity.
- **Status:** Fully validated via KiCad (0 DRC errors, 0 ERC errors) and ready for initial manufacturing and field testing.

### Why a Test PCB?
This patch serves as an experimental prototype to test our core hypothesis: *Can continuous, localized skin temperature monitoring reliably detect autonomic prodromal spikes indicative of an incoming cluster headache attack?*

---

## Community & Collaboration Welcome!

Building open-source medical/health hardware and embedded firmware is a massive undertaking for a single developer. **I cannot do this alone!** 

Whether you are an embedded firmware engineer (Zephyr / nRF), PCB designer (KiCad), mobile app developer (Flutter/Rust), or a fellow cluster headache sufferer / researcher wanting to share insights — **your input, contributions, and feedback are immensely welcome.**

- **Open Issues & PRs:** Feel free to open issues or submit pull requests directly in this repository.
- **Discussions / Contact:** Reach out via GitHub issues or discussions to collaborate on firmware development, clinical validation protocols, or companion app features.

---

## Repository Structure

- `cryo-alert-patch/` — Hardware (KiCad v6 schematics, PCB layout, Gerber files for JLCPCB) & firmware.
- `cryo-mobile-app/` — Flutter companion app with Rust core for real-time $\Delta T/\Delta t$ analysis.
- `cryo-active-cooler/` — Optional Peltier active cooling module concept.

## License

- **Firmware:** MIT
- **Hardware:** CERN-OHL-S-2.0
