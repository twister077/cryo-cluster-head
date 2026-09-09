#!/bin/bash
# Run all host-side unit tests for the firmware.
# Compiles and executes tests that verify the dT/dt detection algorithm
# without needing any hardware.

set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
SRC_DIR="$SCRIPT_DIR/../src"

echo "=== dT/dt Unit Tests ==="
gcc -I "$SRC_DIR" -o "$SCRIPT_DIR/test_dtdt" "$SCRIPT_DIR/test_dtdt.c" "$SRC_DIR/dTdt.c" -lm
"$SCRIPT_DIR/test_dtdt"
echo "=== All tests passed ==="
