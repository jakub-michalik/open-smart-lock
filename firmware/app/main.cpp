/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <lib/support/CHIPMem.h>
#include <platform/CHIPDeviceLayer.h>

#include "runtime.h"

LOG_MODULE_REGISTER(omsl, LOG_LEVEL_INF);

using namespace chip;
using namespace chip::DeviceLayer;

int main(void)
{
    LOG_INF("openMatterSmartLock bring-up");

    if (Platform::MemoryInit() != CHIP_NO_ERROR) {
        LOG_ERR("Platform::MemoryInit failed");
        return -1;
    }
    if (PlatformMgr().InitChipStack() != CHIP_NO_ERROR) {
        LOG_ERR("InitChipStack failed");
        return -1;
    }

    return omsl::Runtime::Instance().StartApp();
}

