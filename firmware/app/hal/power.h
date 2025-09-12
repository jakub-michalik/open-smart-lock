/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Power HAL — pure interface for switching auxiliary power rails
 * (servo, sensor front-end) under firmware control. Used to keep
 * non-MCU current draw at zero between operations.
 *
 * Implementations live under firmware/drivers/power/<name>/.
 */

#pragma once

#include <cstdint>

namespace omsl::hal {

enum class PowerRail : uint8_t {
    Actuator = 0,
    Feedback = 1,
};

class IPower {
public:
    virtual ~IPower() = default;

    virtual bool Init() = 0;

    /**
     * Switch the named power rail on or off. Returns true if the
     * rail is configured on this board; false if the request was a
     * no-op because the rail does not exist.
     */
    virtual bool SetRail(PowerRail rail, bool enabled) = 0;

    /**
     * Read the current state of a configured rail.
     */
    virtual bool IsRailEnabled(PowerRail rail) const = 0;
};

}  // namespace omsl::hal
