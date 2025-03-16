/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(omsl, LOG_LEVEL_INF);

int main(void)
{
    LOG_INF("openMatterSmartLock bring-up");
    return 0;
}
