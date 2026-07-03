# Alert Patch — Bill of Materials

| # | Part | Value / P/N | Qty | Cost |
|---|------|-------------|:---:|:----:|
| 1 | MCU | nRF52832 (QFN-48) | 1 | $5.00 |
| 2 | Temp Sensor | MAX30205 (TDFN-8) | 1 | $3.50 |
| 3 | Battery | CR1220 (40 mAh) | 1 | $1.00 |
| 4 | Battery Holder | CR1220 SMD | 1 | $0.50 |
| 5 | Capacitor | 100nF 0603 | 2 | $0.10 |
| 6 | Capacitor | 10µF 0603 | 1 | $0.15 |
| 7 | Resistor | 10kΩ 0603 | 2 | $0.10 |
| 8 | PCB | 2-layer, 20×20mm | 1 | $2.00 |
| 9 | Enclosure | TPU 3D-print | 1 | $1.00 |
| 10 | Adhesive | CGM patch (×10) | 1 | $5.00 |

**Total: ~$18.35** (excl. setup fees, shipping & VAT)

---

## Production & Cost Indication (NL/Europe)

For initial prototyping and fabrication, the designs are optimized for ordering via **JLCPCB** (or similar PCB fabrication services like PCBWay). Because of the tiny SMD components (QFN-48 MCU and TDFN-8 temperature sensor), manual soldering is extremely difficult. We highly recommend ordering a **fully assembled PCB (PCBA)**.

### Option A: Fully Assembled by JLCPCB (Recommended)
JLCPCB manufactures the board and solders the components automatically. Indicative pricing for a minimum batch of **5 pieces** delivered to the Netherlands:

| Cost Item | Cost (approx. EUR) | Description |
|-----------|--------------------|-------------|
| **PCB Fabrication** | €2.00 | 5 pieces of 20×20 mm 2-layer PCB |
| **SMT Assembly Setup & Stencil** | €15.00 | Stencil creation and machine calibration |
| **Components (5 boards)** | €42.00 | nRF52 MCU, MAX30205 sensor, passives, battery holder |
| **Extended Part Fees** | €5.50 | Loading fees for MCU and sensor |
| **Shipping (EuroPacket)** | €7.50 | Standard shipping with DDP/IOSS to NL |
| **BTW / VAT (21%)** | €15.00 | Paid at checkout via IOSS |
| **Total Price** | **~€85.00 - €95.00** | Approx. **€18.00 per assembled patch** |

> [!TIP]
> **Shipping to the Netherlands**: Select **EuroPacket (DDP/IOSS)** during checkout. This automatically handles Dutch VAT, meaning PostNL/DHL will deliver it directly to your doorstep without any custom delay or extra administration fees.

### Option B: Raw PCBs only (DIY)
If you have access to a reflow hot-plate or stencil oven and want to solder the components yourself:

- **5 Raw PCBs**: ~€2.00
- **Shipping & BTW**: ~€7.00
- **Total Price**: **~€9.00** (excl. component costs)

