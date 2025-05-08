/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <zephyr/kernel.h>

namespace omsl {

class Runtime {
public:
    static Runtime & Instance();

    int StartApp();

private:
    Runtime() = default;
};

}  // namespace omsl
