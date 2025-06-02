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
    Unknown = 2,
};

enum class LockOperation : uint8_t {
    Lock,
    Unlock,
};

class Bolt {
public:
    static Bolt & Instance();

    void Init();

    bool Lock();
    bool Unlock();

    LockState State() const { return mState; }

private:
    Bolt() = default;

    LockState mState { LockState::Unknown };
};

}  // namespace omsl
