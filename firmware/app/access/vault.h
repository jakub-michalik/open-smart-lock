/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "types.h"

namespace omsl::access {

class Vault {
public:
    static Vault & Instance();

    bool Init();
    bool LoadUsers(User * out, size_t max_count);
    bool SaveUsers(const User * users, size_t count);
    bool LoadCredentials(Credential * out, size_t max_count);
    bool SaveCredentials(const Credential * creds, size_t count);

private:
    Vault() = default;
};

}  // namespace omsl::access
