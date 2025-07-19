/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "types.h"

namespace omsl::access {

class Roster {
public:
    static Roster & Instance();

    bool Init();

    bool AddUser(uint16_t index, const char * name);
    bool RemoveUser(uint16_t index);
    bool AddCredential(uint16_t user_index, const Credential & cred);
    bool RemoveCredential(uint16_t user_index, uint16_t credential_index);
    bool AddSchedule(uint16_t user_index, const Schedule & schedule);

    bool ValidatePin(const uint8_t * pin, size_t length, uint16_t * matched_user_out) const;

    void FactoryReset();

private:
    Roster() = default;

    User mUsers[kMaxUsers];
    Credential mCredentials[kMaxUsers * kMaxCredentialsPerUser];
    size_t mCredentialsCount { 0 };
};

}  // namespace omsl::access
