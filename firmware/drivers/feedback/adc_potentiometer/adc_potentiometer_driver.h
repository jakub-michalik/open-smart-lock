/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "../../../app/hal/feedback.h"

namespace omsl::drivers {

class AdcFeedback : public hal::IFeedback {
public:
    static AdcFeedback & Instance();

    bool Init() override;
    hal::FeedbackSample Sample(hal::FeedbackChannel channel) override;
    bool IsChannelAvailable(hal::FeedbackChannel channel) const override;

private:
    AdcFeedback() = default;
};

}  // namespace omsl::drivers
