# Mobile App — Development Guide

## Rust Core

```bash
cd rust-core
cargo test          # Run unit tests
cargo clippy        # Lint
cargo build --release
```

## Flutter App

```bash
cd flutter
flutter pub get
flutter analyze
flutter run
```

## FFI Reference

| Function | Args | Returns | Description |
|----------|------|---------|-------------|
| `cryo_init` | window_size, threshold | 0 = ok | Initialize dT/dt detector |
| `cryo_process_temperature` | temp_c | alert_code | Feed sample, get alert |
| `cryo_reset` | — | 0 = ok | Clear detector state |

Alert codes: `0` = None, `1` = Warning (flare-up detected!), `2` = Critical

## BLE Protocol

The Alert Patch broadcasts temperature in BLE manufacturer data:

| Byte | Field |
|------|-------|
| 0-1 | Manufacturer ID (0x0059) |
| 2-3 | Temperature (°C × 100) |
| 4 | Battery (mV / 10) |
| 5 | Sequence |
| 6 | Status |
