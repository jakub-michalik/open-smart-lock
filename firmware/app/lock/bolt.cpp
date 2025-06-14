/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bolt.h"
#include "../servo_controller.h"

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl {

static ServoController sServo;

Bolt & Bolt::Instance()
{
    static Bolt inst;
    return inst;
}

void Bolt::Init()
{
    mState = LockState::Unknown;
    sServo.Init();
    LOG_INF("Bolt: init");
}

void Bolt::SetStateChangedCallback(LockStateChangedCallback cb)
{
    mCallback = std::move(cb);
}

void Bolt::TransitionTo(LockState newState)
{
    mState = newState;
    if (mCallback) {
        mCallback(newState);
    }
}

bool Bolt::Lock()
{
    if (mState == LockState::Locked || mState == LockState::LockingInitiated) {
        return false;
    }
    TransitionTo(LockState::LockingInitiated);
    if (!sServo.MoveTo(1)) {
        return false;
    }
    TransitionTo(LockState::LockingCompleted);
    TransitionTo(LockState::Locked);
    return true;
}

bool Bolt::Unlock()
{
    if (mState == LockState::Unlocked || mState == LockState::UnlockingInitiated) {
        return false;
    }
    TransitionTo(LockState::UnlockingInitiated);
    if (!sServo.MoveTo(0)) {
        return false;
    }
    TransitionTo(LockState::UnlockingCompleted);
    TransitionTo(LockState::Unlocked);
    return true;
}

}  // namespace omsl
