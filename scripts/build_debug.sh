#!/usr/bin/env bash
# Copyright (c) 2025 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Build the openMatterSmartLock firmware (debug profile).
# Expects a configured west workspace pointed at by NCS_DIR (default:
# $HOME/ncs/v3.1.1). Override BOARD to target something other than the
# nrf52840 DK.

set -euo pipefail

NCS_DIR="${NCS_DIR:-$HOME/ncs/v3.1.1}"
BOARD="${BOARD:-nanoBoard}"
APP_DIR="$(cd "$(dirname "$0")/.." && pwd)/firmware"
BUILD_DIR="${APP_DIR}/../build"

usage() {
    cat <<EOF
Usage: $(basename "$0") [--pristine] [--help]

Environment:
  NCS_DIR  NCS workspace root (default: \$HOME/ncs/v3.1.1)
  BOARD    target board (default: nanoBoard)
EOF
}

pristine=0
for arg in "$@"; do
    case "${arg}" in
        --pristine) pristine=1 ;;
        --help|-h) usage; exit 0 ;;
        *) echo "Unknown option: ${arg}" >&2; usage >&2; exit 1 ;;
    esac
done

if [[ ! -d "${NCS_DIR}/.west" ]]; then
    echo "Missing west workspace at ${NCS_DIR}." >&2
    exit 1
fi

export CMAKE_BUILD_PARALLEL_LEVEL="$(nproc)"

cd "${NCS_DIR}"

build_args=()
[[ ${pristine} -eq 1 ]] && build_args+=(--pristine)

west build --build-dir "${BUILD_DIR}" "${APP_DIR}" "${build_args[@]}" --board "${BOARD}" --sysbuild -- -DCONF_FILE="prj.conf"
