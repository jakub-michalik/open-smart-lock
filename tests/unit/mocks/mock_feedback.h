/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "hal/feedback.h"

namespace omsl::test {

class MockFeedback : public hal::IFeedback {
public:
    bool Init() override { return true; }

    hal::FeedbackSample Sample(hal::FeedbackChannel channel) override {
        hal::FeedbackSample s {};
        s.channel = channel;
        s.value_millivolts = mNextValue;
        s.valid = true;
        return s;
    }

    bool IsChannelAvailable(hal::FeedbackChannel) const override { return true; }

    void SetNextValue(int32_t mv) { mNextValue = mv; }

private:
    int32_t mNextValue { 3300 };
};

}  // namespace omsl::test
