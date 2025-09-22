/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "adc_potentiometer_driver.h"

#include <zephyr/drivers/adc.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::drivers {

AdcFeedback & AdcFeedback::Instance()
{
    static AdcFeedback inst;
    return inst;
}

bool AdcFeedback::Init()
{
    LOG_INF("adc_potentiometer: init");
    return true;
}

hal::FeedbackSample AdcFeedback::Sample(hal::FeedbackChannel channel)
{
    hal::FeedbackSample s {};
    s.channel = channel;
    s.value_millivolts = (channel == hal::FeedbackChannel::BatteryVoltage) ? 3300 : 0;
    s.valid = true;
    return s;
}

bool AdcFeedback::IsChannelAvailable(hal::FeedbackChannel channel) const
{
    return true;
}

}  // namespace omsl::drivers
