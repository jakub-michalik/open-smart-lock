/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vault.h"

#include <zephyr/settings/settings.h>
#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

namespace omsl::access {

Vault & Vault::Instance()
{
    static Vault inst;
    return inst;
}

bool Vault::Init()
{
    int rc = settings_subsys_init();
    if (rc) {
        LOG_ERR("settings_subsys_init rc=%d", rc);
        return false;
    }
    return true;
}

bool Vault::LoadUsers(User * out, size_t max_count)
{
    // Snapshot read from settings — implementation hook
    return true;
}

bool Vault::SaveUsers(const User * users, size_t count)
{
    // Snapshot write to settings — implementation hook
    return true;
}

bool Vault::LoadCredentials(Credential * out, size_t max_count)
{
    return true;
}

bool Vault::SaveCredentials(const Credential * creds, size_t count)
{
    return true;
}

}  // namespace omsl::access
