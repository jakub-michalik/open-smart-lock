/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * PWM servo implementation of osl::omsl::hal::IActuator.
 */

#pragma once

#include "../../../app/hal/actuator.h"

namespace omsl::drivers {

class ServoPwmActuator : public hal::IActuator {
public:
    static ServoPwmActuator & Instance();

    bool Init() override;
    bool MoveTo(hal::ActuatorTarget target, hal::ActuatorCompletionCallback cb) override;
    void Stop() override;
    bool IsBusy() const override { return mBusy; }
    uint32_t MaxActuationTimeMs() const override { return mTimeoutMs; }

private:
    ServoPwmActuator() = default;

    bool mBusy { false };
    uint32_t mTimeoutMs { 1500 };
};

}  // namespace omsl::drivers
