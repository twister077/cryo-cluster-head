# Active Cooler — Assembly & Safety

## PCB Assembly
1. Reflow nRF52832 + CH224K + passives.
2. Solder USB-C receptacle, IRLZ44N, 1N5819.
3. Solder local MAX30205 (position near cold side).
4. Attach Peltier to heatsink with thermal epoxy.
5. Wire fan to 5V header.

## First Test
1. Flash firmware: `west flash`
2. Connect USB-PD power bank — LED indicates PD negotiation.
3. Place near an active Alert Patch — verify BLE connection starts.
4. Warm the local sensor with finger — verify level transitions.

## Level Indicator
- **Level 0:** Fan off, Peltier off
- **Level 1:** Fan low, gentle cooling
- **Level 2:** Fan medium, active cooling
- **Level 3:** Fan max, full Peltier
- **Flashing LED:** Safety fault — check temp sources

## Safety Rules
- Hard cutoff at 15 minutes (regardless of level)
- Auto stop if temp drops below threshold
- Safe shutdown if <2 temperature sources agree
- Immediate stop if USB is unplugged (hardware kill)
