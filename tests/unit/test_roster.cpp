/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>

#include "access/roster.h"

using namespace omsl::access;

ZTEST_SUITE(roster, NULL, NULL, NULL, NULL, NULL);

ZTEST(roster, add_then_remove_user)
{
    Roster::Instance().Init();
    zassert_true(Roster::Instance().AddUser(0, "alice"));
    zassert_true(Roster::Instance().RemoveUser(0));
}

ZTEST(roster, validate_pin_matches_added_credential)
{
    Roster::Instance().Init();
    Roster::Instance().AddUser(0, "bob");

    Credential cred {};
    cred.user_index = 0;
    cred.credential_index = 0;
    cred.type = 0;
    const uint8_t pin[] = {1,2,3,4};
    memcpy(cred.data, pin, 4);
    cred.data_length = 4;
    zassert_true(Roster::Instance().AddCredential(0, cred));

    uint16_t matched = 0xFFFF;
    zassert_true(Roster::Instance().ValidatePin(pin, 4, &matched));
    zassert_equal(matched, 0);
}

ZTEST(roster, validate_pin_rejects_unknown)
{
    Roster::Instance().Init();
    const uint8_t pin[] = {9,9,9,9};
    zassert_false(Roster::Instance().ValidatePin(pin, 4, nullptr));
}

ZTEST(roster, factory_reset_clears_users_and_credentials)
{
    Roster::Instance().Init();
    Roster::Instance().AddUser(0, "alice");
    Roster::Instance().FactoryReset();

    const uint8_t any[] = {1,2,3,4};
    zassert_false(Roster::Instance().ValidatePin(any, 4, nullptr));
}
