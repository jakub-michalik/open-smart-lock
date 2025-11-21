#!/usr/bin/env bash
# Copyright (c) 2025 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Print the first attached Nordic dev-kit serial number via nrfutil.

set -euo pipefail

if ! command -v nrfutil >/dev/null 2>&1; then
    echo "nrfutil not on PATH" >&2
    exit 1
fi

nrfutil device list --traits jlink --serial-number 2>/dev/null \
    | awk '/^Serial number/{print $3; exit}'
