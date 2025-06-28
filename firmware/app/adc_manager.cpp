/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "adc_manager.h"

#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl {

bool AdcManager::Init()
{
    LOG_INF("AdcManager: init");
    return true;
}

int32_t AdcManager::SampleBatteryMillivolts()
{
    // Stub — real ADC read happens through the platform ADC API
    return 3300;
}

}  // namespace omsl
