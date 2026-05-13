#!/usr/bin/env bash
# Copyright (c) 2025 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Release-profile build wrapper. Same arguments as build_debug.sh,
# but selects prj_release.conf.

set -euo pipefail

NCS_DIR="${NCS_DIR:-$HOME/ncs/v3.1.1}"
BOARD="${BOARD:-nanoBoard}"
APP_DIR="$(cd "$(dirname "$0")/.." && pwd)/firmware"
BUILD_DIR="${APP_DIR}/../build_release"

pristine=0
for arg in "$@"; do
    case "${arg}" in
        --pristine) pristine=1 ;;
        --help|-h) echo "Release build wrapper. NCS_DIR / BOARD env vars supported."; exit 0 ;;
    esac
done

export CMAKE_BUILD_PARALLEL_LEVEL="$(nproc)"

cd "${NCS_DIR}"
build_args=()
[[ ${pristine} -eq 1 ]] && build_args+=(--pristine)

west build --build-dir "${BUILD_DIR}" "${APP_DIR}" "${build_args[@]}" --board "${BOARD}" --sysbuild -- -DCONF_FILE="prj_release.conf"
