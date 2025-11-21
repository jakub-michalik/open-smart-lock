#!/usr/bin/env bash
# Copyright (c) 2025 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Flash a previously built debug image. Usage:
#   ./scripts/flash_debug.sh [dev-id|auto] [dump-seconds] [serial-dev] [baud]

set -euo pipefail

NCS_DIR="${NCS_DIR:-$HOME/ncs/v3.1.1}"
APP_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="${APP_DIR}/build"
DEV_ID="${1:-auto}"
DUMP_SECONDS="${2:-}"
DUMP_DEV="${3:-/dev/ttyUSB0}"
DUMP_BAUD="${4:-115200}"

if [[ "${DEV_ID}" == "auto" || -z "${DEV_ID}" ]]; then
    DEV_ID="$("${APP_DIR}/scripts/find_dev_id.sh")"
fi

cd "${NCS_DIR}"
west flash -d "${BUILD_DIR}" --dev-id "${DEV_ID}" --erase

if [[ -n "${DUMP_SECONDS}" ]]; then
    "${APP_DIR}/scripts/serial_dump.sh" "${DUMP_SECONDS}" "${DUMP_DEV}" "${DUMP_BAUD}"
fi
