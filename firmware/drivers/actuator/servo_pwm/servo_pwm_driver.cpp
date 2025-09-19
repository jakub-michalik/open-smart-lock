/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "servo_pwm_driver.h"

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::drivers {

static const struct pwm_dt_spec sServoPwm =
    PWM_DT_SPEC_GET_OR(DT_NODELABEL(servo_pwm), {});

ServoPwmActuator & ServoPwmActuator::Instance()
{
    static ServoPwmActuator inst;
    return inst;
}

bool ServoPwmActuator::Init()
{
    if (!device_is_ready(sServoPwm.dev)) {
        LOG_ERR("servo_pwm: device not ready");
        return false;
    }
    LOG_INF("servo_pwm: init OK");
    return true;
}

bool ServoPwmActuator::MoveTo(hal::ActuatorTarget target, hal::ActuatorCompletionCallback cb)
{
    if (mBusy) return false;
    mBusy = true;
    uint32_t pulse_ns = (target == hal::ActuatorTarget::Locked) ? 1000000 : 2000000;
    int rc = pwm_set_dt(&sServoPwm, 20000000, pulse_ns);
    mBusy = false;
    if (cb) {
        cb(rc == 0 ? hal::ActuatorResult::Success : hal::ActuatorResult::Stalled);
    }
    return rc == 0;
}

void ServoPwmActuator::Stop()
{
    pwm_set_dt(&sServoPwm, 20000000, 0);
    mBusy = false;
}

}  // namespace omsl::drivers
