/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "roster.h"

#include <cstring>

namespace omsl::access {

bool Roster::AddUser(uint16_t index, const char * name)
{
    if (index >= kMaxUsers) return false;
    mUsers[index].index = index;
    mUsers[index].active = true;
    strncpy(mUsers[index].name, name, sizeof(mUsers[index].name) - 1);
    mUsers[index].name[sizeof(mUsers[index].name) - 1] = '\0';
    return true;
}

bool Roster::RemoveUser(uint16_t index)
{
    if (index >= kMaxUsers) return false;
    memset(&mUsers[index], 0, sizeof(mUsers[index]));
    return true;
}

}  // namespace omsl::access
