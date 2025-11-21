#!/usr/bin/env bash
# Copyright (c) 2025 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Read N seconds of serial output from a device and print to stdout.

set -euo pipefail

DUR="${1:-10}"
DEV="${2:-/dev/ttyUSB0}"
BAUD="${3:-115200}"

stty -F "${DEV}" "${BAUD}" cs8 -cstopb -parenb raw -echo
timeout "${DUR}" cat "${DEV}"
