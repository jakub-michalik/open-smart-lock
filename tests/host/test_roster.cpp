/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <doctest/doctest.h>

#include <cstring>

#include "access/roster.h"

using namespace omsl::access;

TEST_CASE("roster: add then remove user")
{
    Roster::Instance().Init();
    REQUIRE(Roster::Instance().AddUser(0, "alice"));
    REQUIRE(Roster::Instance().RemoveUser(0));
}

TEST_CASE("roster: validate PIN matches added credential")
{
    Roster::Instance().Init();
    Roster::Instance().AddUser(0, "bob");

    Credential cred {};
    cred.user_index = 0;
    cred.credential_index = 0;
    cred.type = 0;
    const uint8_t pin[] = { 1, 2, 3, 4 };
    std::memcpy(cred.data, pin, sizeof(pin));
    cred.data_length = sizeof(pin);
    REQUIRE(Roster::Instance().AddCredential(0, cred));

    uint16_t matched = 0xFFFF;
    REQUIRE(Roster::Instance().ValidatePin(pin, sizeof(pin), &matched));
    REQUIRE(matched == 0);
}

TEST_CASE("roster: validate PIN rejects unknown")
{
    Roster::Instance().Init();
    const uint8_t pin[] = { 9, 9, 9, 9 };
    REQUIRE_FALSE(Roster::Instance().ValidatePin(pin, sizeof(pin), nullptr));
}

TEST_CASE("roster: factory reset clears users and credentials")
{
    Roster::Instance().Init();
    Roster::Instance().AddUser(0, "alice");
    Roster::Instance().FactoryReset();

    const uint8_t any[] = { 1, 2, 3, 4 };
    REQUIRE_FALSE(Roster::Instance().ValidatePin(any, sizeof(any), nullptr));
}
