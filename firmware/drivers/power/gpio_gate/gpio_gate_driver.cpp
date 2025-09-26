/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "gpio_gate_driver.h"

#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::drivers {

static const struct gpio_dt_spec sActuatorGate =
    GPIO_DT_SPEC_GET_OR(DT_NODELABEL(power_gate_actuator), gpios, {0});

GpioGatePower & GpioGatePower::Instance()
{
    static GpioGatePower inst;
    return inst;
}

bool GpioGatePower::Init()
{
    if (sActuatorGate.port) {
        gpio_pin_configure_dt(&sActuatorGate, GPIO_OUTPUT_INACTIVE);
    }
    return true;
}

bool GpioGatePower::SetRail(hal::PowerRail rail, bool enabled)
{
    if (rail == hal::PowerRail::Actuator && sActuatorGate.port) {
        gpio_pin_set_dt(&sActuatorGate, enabled ? 1 : 0);
        mActuatorOn = enabled;
        return true;
    }
    if (rail == hal::PowerRail::Feedback) {
        mFeedbackOn = enabled;
        return true;
    }
    return false;
}

bool GpioGatePower::IsRailEnabled(hal::PowerRail rail) const
{
    return (rail == hal::PowerRail::Actuator) ? mActuatorOn : mFeedbackOn;
}

}  // namespace omsl::drivers
