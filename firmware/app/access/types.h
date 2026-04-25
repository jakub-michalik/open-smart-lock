/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include <array>

namespace omsl::access {

constexpr size_t kMaxUsers = 10;
constexpr size_t kMaxCredentialsPerUser = 4;
constexpr size_t kMaxPinLength = 16;

struct User {
    uint16_t index;
    bool active;
    char name[32];
};

struct Credential {
    uint16_t user_index;
    uint16_t credential_index;
    uint8_t type;
    uint8_t data[kMaxPinLength];
    uint8_t data_length;
};

struct Schedule {
    uint16_t user_index;
    uint8_t schedule_index;
    uint32_t start_time;
    uint32_t end_time;
};

}  // namespace omsl::access
