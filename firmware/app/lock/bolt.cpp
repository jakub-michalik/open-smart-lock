/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bolt.h"

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl {

Bolt & Bolt::Instance()
{
    static Bolt inst;
    return inst;
}

void Bolt::Init(hal::IActuator * actuator)
{
    mActuator = actuator;
    mState = LockState::Unknown;
    if (mActuator) {
        mActuator->Init();
    }
    LOG_INF("Bolt: init (actuator=%s)", actuator ? "set" : "null");
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

void Bolt::OnActuatorComplete(hal::ActuatorResult result, LockState completed_state, LockState final_state)
{
    if (result == hal::ActuatorResult::Success) {
        TransitionTo(completed_state);
        TransitionTo(final_state);
    } else {
        LOG_WRN("Actuator failed (%u)", static_cast<unsigned>(result));
        TransitionTo(LockState::Unknown);
    }
}

bool Bolt::Lock()
{
    if (!mActuator) return false;
    if (mState == LockState::Locked || mState == LockState::LockingInitiated) return false;

    TransitionTo(LockState::LockingInitiated);
    return mActuator->MoveTo(hal::ActuatorTarget::Locked,
        [this](hal::ActuatorResult r) {
            OnActuatorComplete(r, LockState::LockingCompleted, LockState::Locked);
        });
}

bool Bolt::Unlock()
{
    if (!mActuator) return false;
    if (mState == LockState::Unlocked || mState == LockState::UnlockingInitiated) return false;

    TransitionTo(LockState::UnlockingInitiated);
    return mActuator->MoveTo(hal::ActuatorTarget::Unlocked,
        [this](hal::ActuatorResult r) {
            OnActuatorComplete(r, LockState::UnlockingCompleted, LockState::Unlocked);
        });
}

}  // namespace omsl
