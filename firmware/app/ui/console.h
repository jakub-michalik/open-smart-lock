/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <functional>

namespace omsl::ui {

using ButtonEventCallback = std::function<void()>;

class Console {
public:
    static Console & Instance();

    bool Init();

    void SetActionButtonCallback(ButtonEventCallback cb);

    void SetStatusLed(bool on);
    void BlinkStatusLed(uint32_t period_ms);
};

}  // namespace omsl::ui
