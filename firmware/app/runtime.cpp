/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "runtime.h"

#include <zephyr/logging/log.h>

#include "ui/console.h"
#include "hal/feedback.h"
#include "lock/bolt.h"

#if defined(CONFIG_OMSL_ACTUATOR_SERVO_PWM)
#include "../drivers/actuator/servo_pwm/servo_pwm_driver.h"
#endif
#if defined(CONFIG_OMSL_FEEDBACK_ADC_POTENTIOMETER)
#include "../drivers/feedback/adc_potentiometer/adc_potentiometer_driver.h"
#endif

LOG_MODULE_DECLARE(omsl);

namespace omsl {

constexpr int32_t kHeartbeatIntervalMs = 5000;

Runtime & Runtime::Instance()
{
    static Runtime instance;
    return instance;
}

int Runtime::StartApp()
{
    LOG_INF("Runtime::StartApp");

    hal::IActuator * actuator = nullptr;
    hal::IFeedback * feedback = nullptr;

#if defined(CONFIG_OMSL_ACTUATOR_SERVO_PWM)
    actuator = &drivers::ServoPwmActuator::Instance();
#endif
#if defined(CONFIG_OMSL_FEEDBACK_ADC_POTENTIOMETER)
    feedback = &drivers::AdcFeedback::Instance();
    feedback->Init();
#endif

    Bolt::Instance().Init(actuator);
    ui::Console::Instance().Init();

    // TODO: wire access::Roster and access::Vault here so that
    // PIN-based unlock and persisted credentials are reachable from
    // the Door Lock cluster callbacks. Currently the lock path is
    // PIN-less and credentials are RAM-only.

    // TODO: wire drivers::GpioGatePower into the actuator path so the
    // servo rail powers up only during MoveTo() and gates off after
    // the completion callback. Today the rail stays on for life.

    ui::Console::Instance().SetActionButtonCallback([]() {
        auto & lm = Bolt::Instance();
        if (lm.State() == LockState::Locked) lm.Unlock();
        else lm.Lock();
    });

    mFeedback = feedback;
    k_work_init_delayable(&mHeartbeat, HeartbeatHandler);
    StartHeartbeat();

    while (true) {
        k_sleep(K_FOREVER);
    }
    return 0;
}

void Runtime::StartHeartbeat()
{
    k_work_schedule(&mHeartbeat, K_MSEC(kHeartbeatIntervalMs));
}

void Runtime::HeartbeatHandler(struct k_work * work)
{
    auto * fb = Instance().mFeedback;
    if (fb) {
        auto s = fb->Sample(hal::FeedbackChannel::BatteryVoltage);
        // TODO: report battery to Matter Power Source cluster instead
        // of just logging; today the cluster attribute is stale
        LOG_INF("Heartbeat: battery=%d mV (valid=%d)", s.value_millivolts, s.valid);
    }
    k_work_schedule(&Instance().mHeartbeat, K_MSEC(kHeartbeatIntervalMs));
}

}  // namespace omsl

// Heartbeat is owned by Runtime and uses k_work_delayable so that the
// timer queue, not a dedicated thread, drives sampling.
