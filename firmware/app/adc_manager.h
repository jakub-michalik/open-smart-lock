/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * ADC manager — battery voltage sampling.
 */

#pragma once

#include <cstdint>

namespace omsl {

class AdcManager {
public:
    bool Init();
    int32_t SampleBatteryMillivolts();
};

}  // namespace omsl
