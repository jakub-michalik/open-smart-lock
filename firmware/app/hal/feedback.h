/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Feedback HAL — pure interface for sensing actuator or environmental
 * state (battery voltage, actuator position, servo feedback line).
 *
 * Implementations live under firmware/drivers/feedback/<name>/.
 * No domain-layer code may include a driver header directly.
 */

#pragma once

#include <cstdint>
#include <cstddef>

namespace omsl::hal {

enum class FeedbackChannel : uint8_t {
    BatteryVoltage = 0,
    ActuatorPosition = 1,
};

struct FeedbackSample {
    FeedbackChannel channel;
    int32_t value_millivolts;  // raw mV for analog channels
    bool valid;
};

class IFeedback {
public:
    virtual ~IFeedback() = default;

    /**
     * Initialize the underlying hardware. Returns true on success.
     */
    virtual bool Init() = 0;

    /**
     * Synchronously sample a single channel. Implementations are expected
     * to power-gate the analog front-end so the sensor draws current
     * only during the sampling window.
     */
    virtual FeedbackSample Sample(FeedbackChannel channel) = 0;

    /**
     * True if the channel is configured and available on this hardware.
     */
    virtual bool IsChannelAvailable(FeedbackChannel channel) const = 0;
};

}  // namespace omsl::hal
