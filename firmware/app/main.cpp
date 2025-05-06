/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <lib/support/CHIPMem.h>
#include <platform/CHIPDeviceLayer.h>

LOG_MODULE_REGISTER(omsl, LOG_LEVEL_INF);

using namespace chip;
using namespace chip::DeviceLayer;

int main(void)
{
    LOG_INF("openMatterSmartLock bring-up");

    CHIP_ERROR err = Platform::MemoryInit();
    if (err != CHIP_NO_ERROR) {
        LOG_ERR("Platform::MemoryInit failed: %" CHIP_ERROR_FORMAT, err.Format());
        return -1;
    }

    err = PlatformMgr().InitChipStack();
    if (err != CHIP_NO_ERROR) {
        LOG_ERR("InitChipStack failed: %" CHIP_ERROR_FORMAT, err.Format());
        return -1;
    }

    LOG_INF("CHIP stack initialized");
    return 0;
}
