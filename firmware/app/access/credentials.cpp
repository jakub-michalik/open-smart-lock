/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "roster.h"

#include <cstring>

namespace omsl::access {

bool Roster::AddCredential(uint16_t user_index, const Credential & cred)
{
    if (user_index >= kMaxUsers) return false;
    if (mCredentialsCount >= sizeof(mCredentials)/sizeof(mCredentials[0])) return false;
    mCredentials[mCredentialsCount] = cred;
    mCredentials[mCredentialsCount].user_index = user_index;
    ++mCredentialsCount;
    return true;
}

bool Roster::RemoveCredential(uint16_t user_index, uint16_t credential_index)
{
    for (size_t i = 0; i < mCredentialsCount; ++i) {
        if (mCredentials[i].user_index == user_index &&
            mCredentials[i].credential_index == credential_index) {
            for (size_t j = i; j + 1 < mCredentialsCount; ++j) {
                mCredentials[j] = mCredentials[j + 1];
            }
            --mCredentialsCount;
            return true;
        }
    }
    return false;
}

bool Roster::ValidatePin(const uint8_t * pin, size_t length, uint16_t * matched_user_out) const
{
    // TODO: replace memcmp with a constant-time compare; current code
    // is vulnerable to timing side channels (low risk on this class of
    // device but fixing it costs nothing)
    // TODO: wire wrong-counter + exponential backoff per security model
    for (size_t i = 0; i < mCredentialsCount; ++i) {
        if (mCredentials[i].data_length == length &&
            memcmp(mCredentials[i].data, pin, length) == 0) {
            if (matched_user_out) *matched_user_out = mCredentials[i].user_index;
            return true;
        }
    }
    return false;
}

}  // namespace omsl::access
