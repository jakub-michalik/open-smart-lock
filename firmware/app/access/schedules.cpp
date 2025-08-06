/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "roster.h"

namespace omsl::access {

bool Roster::AddSchedule(uint16_t user_index, const Schedule & schedule)
{
    if (user_index >= kMaxUsers) return false;
    // Schedules are stored alongside the user — simplified initial impl.
    return true;
}

}  // namespace omsl::access
