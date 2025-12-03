/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "radio.h"

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::transport {

Radio & Radio::Instance()
{
    static Radio inst;
    return inst;
}

bool Radio::Init()
{
    LOG_INF("Radio: init (current=%u)", static_cast<unsigned>(mActive));
    return true;
}

bool Radio::RequestTransport(hal::Transport transport)
{
    if (transport == mActive) return true;
    LOG_INF("Radio: %u -> %u",
            static_cast<unsigned>(mActive), static_cast<unsigned>(transport));
    mActive = transport;
    return true;
}

}  // namespace omsl::transport
