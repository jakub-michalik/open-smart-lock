/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "bolt.h"
#include "../access/roster.h"

namespace omsl {

bool ValidateRemotePin(const uint8_t * pin, uint8_t length)
{
    return access::Roster::Instance().ValidatePin(pin, length, nullptr);
}

}  // namespace omsl
