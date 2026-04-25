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
    // TODO: implement settings_load_subtree("omsl/users", ...) and
    // unpack each user record. Currently a stub for tests; lock works
    // because Bolt does not yet read users at runtime.
    return true;
}

bool Vault::SaveUsers(const User * users, size_t count)
{
    // TODO: implement settings_save_one("omsl/users/<i>", ...) per
    // active user; serialize User struct as a packed POD blob
    return true;
}

bool Vault::LoadCredentials(Credential * out, size_t max_count)
{
    // TODO: same shape as LoadUsers, subtree "omsl/credentials"
    return true;
}

bool Vault::SaveCredentials(const Credential * creds, size_t count)
{
    // TODO: same shape as SaveUsers, subtree "omsl/credentials"
    return true;
}

}  // namespace omsl::access
