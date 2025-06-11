/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "servo_controller.h"

#include <zephyr/device.h>
#include <zephyr/drivers/pwm.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl {

static const struct pwm_dt_spec servo_pwm =
    PWM_DT_SPEC_GET_OR(DT_NODELABEL(servo_pwm), {});

bool ServoController::Init()
{
    if (!device_is_ready(servo_pwm.dev)) {
        LOG_ERR("ServoController: PWM device not ready");
        return false;
    }
    mInitialized = true;
    LOG_INF("ServoController: init OK");
    return true;
}

bool ServoController::MoveTo(uint8_t target_state)
{
    if (!mInitialized) return false;
    // Locked: 1.0 ms pulse, Unlocked: 2.0 ms pulse
    uint32_t pulse_ns = (target_state == 0) ? 2000000 : 1000000;
    int rc = pwm_set_dt(&servo_pwm, 20000000, pulse_ns);
    if (rc) {
        LOG_ERR("ServoController: pwm_set_dt rc=%d", rc);
        return false;
    }
    return true;
}

void ServoController::Stop()
{
    pwm_set_dt(&servo_pwm, 20000000, 0);
}

}  // namespace omsl
