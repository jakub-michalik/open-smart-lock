/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/kernel.h>

#include "hal/feedback.h"

namespace omsl {

class Runtime {
public:
    static Runtime & Instance();

    int StartApp();

private:
    Runtime() = default;

    void StartHeartbeat();
    static void HeartbeatHandler(struct k_work * work);

    struct k_work_delayable mHeartbeat;
    hal::IFeedback * mFeedback { nullptr };
};

}  // namespace omsl
