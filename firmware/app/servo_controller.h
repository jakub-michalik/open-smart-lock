/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Servo controller — first pass. Direct call from Bolt.
 * Will be refactored behind a HAL interface later.
 */

#pragma once

#include <cstdint>

namespace omsl {

class ServoController {
public:
    bool Init();
    bool MoveTo(uint8_t target_state);
    void Stop();
private:
    bool mInitialized { false };
};

}  // namespace omsl
