/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <functional>

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

enum class LockOperation : uint8_t {
    Lock,
    Unlock,
};

using LockStateChangedCallback = std::function<void(LockState)>;

class Bolt {
public:
    static Bolt & Instance();

    void Init();
    void SetStateChangedCallback(LockStateChangedCallback cb);

    bool Lock();
    bool Unlock();

    LockState State() const { return mState; }

private:
    Bolt() = default;

    void TransitionTo(LockState newState);

    LockState mState { LockState::Unknown };
    LockStateChangedCallback mCallback;
};

}  // namespace omsl

namespace omsl {

constexpr uint32_t kServoLockedPulseUs = 1000;
constexpr uint32_t kServoUnlockedPulseUs = 2000;
constexpr uint32_t kServoActuationTimeoutMs = 1500;

}  // namespace omsl
