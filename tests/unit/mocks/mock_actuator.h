/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "hal/actuator.h"

#include <vector>

namespace omsl::test {

class MockActuator : public hal::IActuator {
public:
    bool Init() override { mInitialized = true; return true; }

    bool MoveTo(hal::ActuatorTarget target, hal::ActuatorCompletionCallback cb) override {
        mCalls.push_back(target);
        if (cb) cb(mNextResult);
        return true;
    }

    void Stop() override { mStopped = true; }
    bool IsBusy() const override { return false; }
    uint32_t MaxActuationTimeMs() const override { return 1500; }

    // Test helpers
    void SetNextResult(hal::ActuatorResult r) { mNextResult = r; }
    const std::vector<hal::ActuatorTarget> & Calls() const { return mCalls; }
    bool Initialized() const { return mInitialized; }
    bool Stopped() const { return mStopped; }

private:
    bool mInitialized { false };
    bool mStopped { false };
    hal::ActuatorResult mNextResult { hal::ActuatorResult::Success };
    std::vector<hal::ActuatorTarget> mCalls;
};

}  // namespace omsl::test

