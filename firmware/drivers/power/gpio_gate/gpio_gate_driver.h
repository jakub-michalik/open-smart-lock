/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "../../../app/hal/power.h"

namespace omsl::drivers {

class GpioGatePower : public hal::IPower {
public:
    static GpioGatePower & Instance();

    bool Init() override;
    bool SetRail(hal::PowerRail rail, bool enabled) override;
    bool IsRailEnabled(hal::PowerRail rail) const override;

private:
    GpioGatePower() = default;

    bool mActuatorOn { false };
    bool mFeedbackOn { false };
};

}  // namespace omsl::drivers
