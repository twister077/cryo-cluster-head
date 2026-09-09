# CryoClusterHead — Build & Validation Plan

> **Goal:** Validate the theory that cluster headache flare-ups can be detected via skin temperature (dT/dt), by building a working patch and testing it on a real patient.

## Background

Cluster headaches are among the most excruciatingly painful conditions known. This project explores an open-source hardware and software approach to **early detection of cluster headache flare-ups** using continuous skin temperature monitoring. By tracking rapid localized temperature changes (dT/dt) at the temple/neck area during the prodromal phase, our goal is to provide an early warning notification **minutes before peak pain hits**, allowing timely intervention (such as cold therapy or medication).

---

## Current Firmware Status (STM32L011 — `cryo-alert-patch/firmware-stm32/`)

> ✅ = Complete | 🚧 = In Progress | ⬜ = Not started

| Component | Status | Notes |
|-----------|--------|-------|
| TMP117 I2C driver | ✅ | `src/tmp117.c` — temperature sensor on B.Cu (skin contact) |
| ADC battery monitoring | ✅ | VBAT_SENSE on PA0, CR1220 voltage divider |
| dT/dt rate-of-change detection | ✅ | `src/dTdt.c` — rolling window, spike confirmation (3 consecutive high rates) |
| LED/buzzer alert state machine | ✅ | Normal/Warning/Critical/Muted states |
| Button debounce + snooze | ✅ | PB4 input with debounce, 30s mute |
| Low-power sleep | ✅ | `src/power.c` — Sleep mode (WFI) between measurements |
| Host-side unit tests | ✅ | 10/10 pass via `test/run_tests.sh` |
| Build & flash documentation | ✅ | `BUILD.md` — SWD flash via J1 POGO pads |

**Build stats:** RAM 14.6% (300 B), Flash 67.5% (11 KB)

---

## Phase 0 — Project fixes (code + docs)

Before building new features, the existing codebase gets maintenance:

**What:** | **Why:**
---------|--------
Fix CI workflows (`app/rust-core` → `cryo-mobile-app/rust-core`, etc.) | 3 of 6 workflows point to non-existent paths
`main.c` BLE advertisement stub | `update_ble_advertisement()` creates local array but never sends via BLE
Translate documentation to English | Repo documentation should be accessible to all contributors

**Status:** ✅ Documentation translated. Firmware built and validated.

---

## Phase 1 — Validate the current prototype board (STM32L011, TMP117)

The existing v6 PCB needs a thorough review before field testing:

- **Run DRC/ERC again** — confirm the routing was completed, not just the schematic
- **Verify pin assignments** against the STM32L011 datasheet (QFN-32) — ensure correct mapping for I2C, ADC, GPIO, and SWD pins
- **Check the battery positive terminal** — verify VBAT_SENSE routing and proper connection to the CR1220 battery
- **Confirm SWD header** — ensure SWDIO and SWCLK are correctly routed to the J1 POGO pads for flashing

### 1.2 Assemble a first test unit

- **Order PCBA** via JLCPCB (recommended for small SMD components)
- Identify any missing components (e.g., crystal for STM32, decoupling caps)
- Probe all power rails before powering on the MCU

### 1.3 Bench test with a logic analyzer / scope

- Verify **I2C communication** with the TMP117 temperature sensor
- Confirm **LED and buzzer** operation according to the alert state machine
- Measure the **actual power consumption** in low‑power sleep mode

---

## Phase 2 — dT/dt detection firmware

> ✅ Core algorithm complete (`src/dTdt.c`). Unit tests pass. Spike confirmation logic validated.

- ✅ Temperature sampling at 2s interval with rolling window
- ✅ Rate of change (dT/dt) computed on consecutive sample pairs
- ✅ Spike detection: ≥3 consecutive high-rate readings (configurable threshold)
- ✅ LED + buzzer alert on critical temperature or flare-up detection
- ⬜ BLE notification to phone app (requires BLE stack integration)

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

## Open questions / risks

- **Hypothesis validity:** It is not yet proven that cluster headache prodromes cause measurable skin temperature spikes
- **Sensor placement:** Temple vs. neck — which site gives the clearest signal?
- **Threshold tuning:** The 0.5°C/min threshold is a starting point; will need tuning based on real data
- **False positives:** Normal activities (touching the patch, going outdoors) may cause dT/dt spikes unrelated to headaches

## Priority / recommended order

1. ✅ Firmware algorithm complete — ready for hardware testing
2. Build & flash documentation done
3. Next: Assemble test unit (Phase 1.2) and bench test (Phase 1.3)
4. Then: BLE integration (Phase 2 BLE notification)
5. Then: Field test (Phase 4)
