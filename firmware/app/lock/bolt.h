/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <functional>

#include "../hal/actuator.h"

namespace omsl {

enum class LockState : uint8_t {
    Unlocked = 0,
    Locked = 1,
    UnlockingInitiated = 2,
    UnlockingCompleted = 3,
    LockingInitiated = 4,
    LockingCompleted = 5,
    Unknown = 6,
};

using LockStateChangedCallback = std::function<void(LockState)>;

class Bolt {
public:
    static Bolt & Instance();

    void Init(hal::IActuator * actuator);
    void SetStateChangedCallback(LockStateChangedCallback cb);

    bool Lock();
    bool Unlock();

    LockState State() const { return mState; }

private:
    Bolt() = default;

    void TransitionTo(LockState newState);
    void OnActuatorComplete(hal::ActuatorResult result, LockState success_state, LockState completed_state);

    LockState mState { LockState::Unknown };
    LockStateChangedCallback mCallback;
    hal::IActuator * mActuator { nullptr };
};

// PIN validation hook
bool ValidateRemotePin(const uint8_t * pin, uint8_t length);

constexpr uint32_t kServoLockedPulseUs = 1000;
constexpr uint32_t kServoUnlockedPulseUs = 2000;
constexpr uint32_t kServoActuationTimeoutMs = 1500;

}  // namespace omsl
