/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "roster.h"

#include <cstring>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::access {

Roster & Roster::Instance()
{
    static Roster inst;
    return inst;
}

bool Roster::Init()
{
    memset(mUsers, 0, sizeof(mUsers));
    mCredentialsCount = 0;
    LOG_INF("Roster: init");
    return true;
}

void Roster::FactoryReset()
{
    memset(mUsers, 0, sizeof(mUsers));
    memset(mCredentials, 0, sizeof(mCredentials));
    mCredentialsCount = 0;
    LOG_INF("Roster: factory reset");
}

}  // namespace omsl::access
