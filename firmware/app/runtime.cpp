/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "runtime.h"

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl {

Runtime & Runtime::Instance()
{
    static Runtime instance;
    return instance;
}

int Runtime::StartApp()
{
    LOG_INF("Runtime::StartApp");
    while (true) {
        k_sleep(K_FOREVER);
    }
    return 0;
}

}  // namespace omsl
