# CryoClusterHead — Bouw- & Validatieplan

> **Doel:** De theorie valideren dat clusterhoofdpijn-opvlammingen detecteerbaar zijn via huidtemperatuur (dT/dt), door een werkende patch te bouwen en bij een echte patiënt te testen.

---

## Achtergrond

Het project beweert dat clusterhoofdpijn-aanvallen gepaard gaan met een snelle huidtemperatuurstijging op de slaap/nek, en dat een dT/dt-algoritme (rate of change) vroege detectie mogelijk maakt, nog voor de piekpijn. **Dit is een onbewezen hypothese**, niet gebaseerd op peer-reviewed onderzoek. De enige manier om te valideren of dit klopt, is door de hardware te bouwen en bij jezelf te testen.

---

## Fase 0 — Project fixes (code + docs)

Voordat er gebouwd wordt, krijgt de bestaande codebase onderhoud.

**Wat:** | **Waarom:**
---------|----------
CI workflows repareren (`app/rust-core` → `cryo-mobile-app/rust-core`, etc.) | 3 van de 6 workflows verwijzen naar niet-bestaande paden en zullen falen in CI
`main.c` BLE advertisement stub repareren | `update_ble_advertisement()` maakt een lokale array maar verstuurt niets via BLE — de patch zou nooit zichtbaar zijn voor de app
Flutter deprecated API's vervangen (`withKeywords`, `withValues`) | Compileert niet met huidige `flutter_blue_plus` en Flutter SDK versies
`rust_bridge.dart` unused import verwijderen | Schoonhouden
Documentatie bijwerken — theorie → hypothese | Claims als "detects flare-ups before peak pain" → "aims to detect"
Logging-formaat toevoegen voor data-export | Essentieel voor latere validatie-analyse

---

## Fase 1 — Eerste werkende setup (nRF52 DK i.p.v. custom PCB)

Omdat de maker alleen through-hole ervaring heeft en geen nRF52-programmer bezit, wordt een **nRF52832 Development Kit (nRF52 DK)** gebruikt in plaats van de custom PCB (QFN-48 is te uitdagend zonder reflow-ervaring). De programmeerder/debugger zit ingebouwd op de DK.

### Stappen

| # | Actie | Kosten |
|---|-------|--------|
| 1.1 | **nRF52 DK kopen** (Mouser/Digikey — ~€35) | €35 |
| 1.2 | **MAX30205 breakout kopen** (SparkFun MAX30205 breakout of losse TDFN-8 + breakoutboard — ~€10) | €10 |
| 1.3 | Dupont-draden (female-female), breadboard, usb-kabel | €5 |
| 1.4 | **Zephyr toolchain installeren**: Python, West, ARM GCC | gratis |
| 1.5 | MAX30205 aansluiten op DK (I2C: SDA/SCL → DK pin 27/28, 3.3V → pin 15, GND → pin 29) | — |
| 1.6 | Firmware bouwen & flashen | — |
| 1.7 | **Verifiëren** met nRF Connect app (Android/iOS): zie je "CryoPatch" met temperatuurdata? | gratis |

**Totaal fase 1: ~€50**

### Aansluitschema MAX30205 ↔ nRF52 DK

```
MAX30205 Pin     nRF52 DK Pin
────────────────────────────────
VDD        →     3.3V (P15)
GND        →     GND (P29)
SCL        →     P0.27 (P27) — I2C1 SCL
SDA        →     P0.28 (P28) — I2C1 SDA
```

Pull-up weerstanden (10kΩ) op SCL/SDA — op de meeste breakouts al aanwezig, anders via breadboard toevoegen.

---

## Fase 2 — Mobile app werkend krijgen

### Stappen

| # | Actie |
|---|-------|
| 2.1 | Rust toolchain installeren (`rustup`) |
| 2.2 | `cryo-mobile-app/rust-core` builden: `cargo test && cargo build --release` |
| 2.3 | Flutter SDK installeren |
| 2.4 | `cryo-mobile-app/flutter` repareren (deprecated API's) |
| 2.5 | App draaien op Android telefoon: `flutter run` |
| 2.6 | Verbinden met de patch — zie je temperatuur op het dashboard? |

### Bekende problemen

- `dashboard.dart` gebruikt `FlutterBluePlus.startScan(withKeywords: [...])` — deze parameter is verwijderd in recente versies. Oplossing: filteren op `device.platformName` na scan.
- `history.dart` gebruikt `.withValues(alpha: ...)` — vervangen door `.withOpacity(...)` voor oudere Flutter.
- `rust_bridge.dart` importeert `package:ffi/ffi.dart` maar gebruikt het niet.

---

## Fase 3 — Zelf-testen & data loggen (de validatie)

Dit is de kern: wordt de hypothese bevestigd of weerlegd?

### Opzet

- Patch dragen op de slaap (rechterzijde = ipsilateraal bij clusterhoofdpijn)
- 24/7 meten, 1 sample per minuut
- **Elke opvlamming loggen**: tijdstip + ernst (1-10) noteren in app of notitie
- Data verzamelen gedurende minimaal 2 weken (liefst 4)

### Analyse

- Exporteren van temperatuur-tijdreeks
- Visueel inspecteren: zie je een temperatuurstijging vóór elke opvlamming?
- dT/dt parameters tunen:
  - Window size: 3–20 samples (3–20 minuten venster)
  - Threshold: 0.1–2.0 °C/min
  - Debounce count: 1–3
- Valideren: wat is de false-positive rate? Wat is de detectie-rate?

### Criteria

| Resultaat | Conclusie |
|-----------|-----------|
| Temperatuurstijging zichtbaar vóór ≥ 70% van de opvlammingen | Hypothese bevestigd — dT/dt werkt |
| Temperatuurstijging soms, maar niet consistent | Parameters tunen; misschien hogere sample rate (elke 30s) nodig |
| Geen correlatie | Hypothese weerlegd voor deze persoon — alternatieve sensor nodig (NIRS, hartslag, etc.) |

---

## Fase 4 — Project updaten met resultaten

Na de testperiode wordt alles bijgewerkt op basis van echte data:

### Code

- Standaard dT/dt parameters aanpassen (threshold, window size, debounce) op basis van jouw data
- Data-export functionaliteit toevoegen (CSV, JSON)
- Automatische flare-up log toevoegen aan de app
- Historiegrafiek verbeteren met markeringen voor opvlammingen

### Documentatie

- README herschrijven: van "detects flare-ups" → "showed X% detection rate in N=1 self-test"
- Bevindingen documenteren in `docs/validation.md`
- Configuratie-aanbevelingen voor toekomstige gebruikers

### Hardware

- **Alleen bij bewezen werking:** custom PCB (20×20mm) laten fabriceren bij JLCPCB
- TPU 3D-print behuizing ontwerpen
- Componenten bestellen voor de definitieve patchoromot

---

## Tijdsinschatting

| Fase | Tijd | Afhankelijk van |
|------|------|----------------|
| Fase 0 — Code fixes | ~2 uur | — |
| Fase 1 — DK setup | ~3 uur | Bestelling (~1 week levertijd) |
| Fase 2 — Mobile app | ~3 uur | Fase 1 (DK) |
| Fase 3 — Testen | 2–4 weken | Fase 1 + 2 |
| Fase 4 — Project updaten | ~4 uur | Fase 3 resultaten |

**Totale doorlooptijd:** 3–5 weken (excl. wachten op bestellingen)

---

## Benodigdheden

### Hardware (aan te schaffen)

| Item | Geschatte prijs | Waar |
|------|----------------|------|
| nRF52832 DK (PCA10040) | €35 | Mouser, Digikey |
| MAX30205 breakout | €10 | SparkFun, AliExpress |
| Dupont-draden (F/F) | €3 | Lokale elektronicawinkel |
| Breadboard | €3 | - |
| Micro-USB kabel | €2 | - |

### Software (gratis)

- Python 3.x + West (`pip install west`)
- ARM GCC toolchain (Zephyr SDK of gcc-arm-none-eabi)
- nRF Command Line Tools
- Rust (`rustup`)
- Flutter SDK
- nRF Connect app (op telefoon)

### Optioneel (voor fase 4)

- TPU filament voor 3D-printer
- JLCPCB order voor custom PCB
- CR1220 batterij + SMD houder

---

## Risico's & Mitigatie

| Risico | Kans | Mitigatie |
|--------|------|-----------|
| Geen temperatuurstijging detecteerbaar | Middel | Dan is de hypothese weerlegd — nog waardevolle uitkomst. Andere sensoren overwegen (NIRS, hartslagvariabiliteit) |
| MAX30205 I2C werkt niet op breadboard | Klein | 10kΩ pull-ups controleren, adres checken (0x48), logic analyzer gebruiken |
| Flutter BLE werkt niet met nRF52 DK | Klein | nRF Connect app als referentie; als het daarmee werkt, ligt het aan Flutter |
| QFN solderen lukt niet (fase 4 custom PCB) | Groot | Daarom beginnen we met DK. Als custom PCB later nodig is: JLCPCB assembled PCBA bestellen |
| Te weinig opvlammingen in testperiode | Variabel | Testperiode verlengen of eerder verzamelde data gebruiken |

---

## Go / No-go momenten

| Moment | Vraag |
|--------|-------|
| Na fase 1.7 | Werkt de BLE-communicatie? → Zo nee, debuggen. Zo ja, door naar fase 2. |
| Na fase 3 | Is er een correlatie tussen temperatuurstijging en opvlamming? → Zo nee, theorie verwerpen of alternatieve sensor proberen. Zo ja, door naar fase 4. |
| Na fase 4 | Is een custom PCB de moeite waard? → Alleen als de detectie betrouwbaar genoeg is voor dagelijks gebruik. |

---

> **⚠ Experimenteel apparaat. Geen gecertificeerd medisch product. Gebruik op eigen risico.**
> Het doel is om een hypothese te toetsen, niet om een medisch hulpmiddel te bouwen.
