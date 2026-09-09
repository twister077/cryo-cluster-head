# Cryo Mobile App

> Part of the [CryoClusterHead](../README.md) system — open-source cluster headache detection and relief.

**Flutter + Rust app for real-time temperature monitoring and flare-up detection.**

Pairs with the [Cryo Alert Patch](../cryo-alert-patch) via BLE to detect rapid temperature spikes (dT/dt) characteristic of cluster headache attacks. Sends push notifications before the peak pain hits.

> **Note on Project Status:** The mobile application relies on BLE connectivity, which belongs to **Track B (Future / BLE Version)**. The current active hardware and firmware testing track is **Track A (Non-BLE Test Patch)** using the STM32L011 + TMP117.

## Standalone Value

This app works standalone with the Alert Patch — no cooling hardware required:

- **Real-time temperature** — live °C readout from the patch
- **Flare-up detection** — proprietary dT/dt algorithm catches rapid temperature rises
- **Push alerts** — notified during the prodromal phase, minutes before peak pain
- **24h history** — charts and statistics to identify patterns and triggers
- **No cloud** — all processing happens on-device via the Rust core engine

## Architecture

```
┌──────────────┐   BLE    ┌───────────────┐   FFI    ┌──────────────┐
│ Alert Patch  │─────────▶│  Rust Core     │─────────▶│  Flutter UI  │
│ (Track B BLE)│          │  - BLE parser  │          │  - Dashboard │
│              │          │  - dT/dt algo  │          │  - History   │
│              │          │  - Notification│          │  - Settings  │
└──────────────┘          └───────────────┘          └──────────────┘
```

## Quick Start *(Track B - BLE Integration)*

```bash
# Rust core
cd rust-core
cargo test
cargo build --release

# Flutter app
cd flutter
flutter pub get
flutter run
```

## Building for Android / iOS

```bash
# Build Rust for Android
cargo ndk -t arm64-v8a -o flutter/android/app/src/main/jniLibs build --release

# Build Rust for iOS
cargo lipo --release

# Build Flutter
cd flutter
flutter build apk --release    # Android
flutter build ios --release    # iOS
```

## License

MIT
