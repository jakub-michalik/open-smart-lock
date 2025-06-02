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

void Bolt::Init()
{
    mState = LockState::Unknown;
    LOG_INF("Bolt: init");
}

bool Bolt::Lock()
{
    LOG_INF("Bolt: Lock");
    mState = LockState::Locked;
    return true;
}

bool Bolt::Unlock()
{
    LOG_INF("Bolt: Unlock");
    mState = LockState::Unlocked;
    return true;
}

}  // namespace omsl
