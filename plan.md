# CryoClusterHead — Build & Validation Plan

> **Goal:** Validate the theory that cluster headache flare-ups can be detected via skin temperature (dT/dt), by building a working patch and testing it on a real patient.

---

## Background

The project claims that cluster headache attacks are accompanied by a rapid rise in skin temperature at the temple/neck, and that a dT/dt algorithm (rate of change) enables early detection, even before peak pain. **This is an unproven hypothesis**, not based on peer-reviewed research. The only way to validate whether this holds is to build the hardware and test it on yourself.

---

## Phase 0 — Project fixes (code + docs)

Before anything is built, the existing codebase receives maintenance.

**What:** | **Why:**
---------|----------
Fix CI workflows (`app/rust-core` → `cryo-mobile-app/rust-core`, etc.) | 3 of the 6 workflows point to non-existent paths and will fail in CI
Fix the `main.c` BLE advertisement stub | `update_ble_advertisement()` creates a local array but sends nothing over BLE — the patch would never be visible to the app

---

## Phase 1 — Validate & build the hardware

### 1.1 Validate the current prototype board (nRF52832, MAX30205)

The existing v6 PCB needs a thorough review before field testing:

- **Run DRC/ERC again** — confirm the routing was completed, not just the schematic
- **Verify pin assignments** against the actual nRF52832 datasheet (QFN-48) — the earlier analysis found mismatches between schematic nets and physical pins (e.g. GND instead of DEC/XC pins)
- **Check the battery positive terminal** — the via to the VCC_NRF plane was previously routed outside the board boundary
- **Confirm antenna matching** — without a 32 MHz crystal and antenna matching network the chip cannot transmit

### 1.2 Assemble a first test unit

- **Order PCBA** via JLCPCB (recommended for small SMD components)
- identify the Crystal/antenna components that are missing
- Probe all power rails before powering on the MCU

### 1.3 Bench test with a logic analyzer / scope

- Verify **I2C communication** with the temperature sensor
- Confirm **BLE advertisement** packets on a sniffer
- Measure the **actual power consumption** in sleep mode

---

## Phase 2 — dT/dt detection firmware

- Implement **temperature sampling** at a fixed interval (e.g. every 30–60 s) with a rolling window
- Compute the **rate of change (dT/dt)** over the window
- Trigger an **alert** when dT/dt exceeds a configurable threshold for N consecutive samples (reject single spikes)
- Send the alert as a **BLE notification** to the phone app

---

## Phase 3 — Mobile app: real-time monitoring

- Show **live temperature + dT/dt** on the dashboard
- **Push notification** on alert (local, and remote if needed)
- **History graph** so the patient can identify triggers
- Profile/export the logged data for later analysis

---

## Phase 4 — Field test (single user)

- Wear the patch during a **real cluster headache** (or during a few normal days)
- Log all temperature data + timestamps
- Compare with the **known attack timeline** (from the patient's log)
- Determine sensitivity/specificity of the alert (how many seconds before peak pain, how many false positives)

---

## Phase 5 — Community & open science

- Publish the **dataset** (anonymized) of temperature curves
- Write up the **validated hypothesis** as a case report
- Open the results as a **discussion** so others can reproduce and improve the method

---

## Open questions / risks

| Risk | Impact | Mitigation |
|------|--------|------------|
| Hypothesis is wrong (no reliable dT/dt spike) | The whole concept fails | Cost of validation is only 1 PCB + firmware. Do this first. |
| Hardware bugs (pinout, crystal, antenna) | No reading / no transmission | Double-check pinout with datasheet, bench-test before patient |
| Temperature noise | False positives | Require N consecutive samples above threshold |
| Battery too small | Short runtime, patch stops | Measure sleep power; if needed, larger CR2032 or rechargeable |
| BLE range / phone pairing | Missed alerts | Robust scan + reconnect logic in app |

---

## Priorities (what to do first)

1. **Validate the existing PCB** (DRC/ERC, pinout, missing crystal/antenna) — if it is fundamentally broken, a redesign costs less earlier.
2. **Get a working board + firmware** (any sensor, any MCU) — prove skin temp detection works.
3. **Then the app, notifications, and field test.**
4. Anything cosmetic (nice UI, polish) is last.

> **Bottom line:** the fastest path to knowing "does this work?" is a minimal, validated board that samples temperature and reports dT/dt. Everything else is secondary.