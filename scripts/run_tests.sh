#!/usr/bin/env bash
set -euo pipefail

# Run openMatterSmartLock unit tests via twister.

cd "$(dirname "$0")/.."

west twister \
    -T tests/unit \
    -p native_sim \
    --inline-logs \
    "$@"
