/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <doctest/doctest.h>

#include "lock/bolt.h"
#include "mocks/mock_actuator.h"

using namespace omsl;

TEST_CASE("bolt: init with null actuator handles gracefully")
{
    Bolt::Instance().Init(nullptr);
    REQUIRE_FALSE(Bolt::Instance().Lock());
}

TEST_CASE("bolt: lock transitions to Locked on actuator success")
{
    test::MockActuator mock;
    mock.SetNextResult(hal::ActuatorResult::Success);
    Bolt::Instance().Init(&mock);

    REQUIRE(Bolt::Instance().Lock());
    REQUIRE(Bolt::Instance().State() == LockState::Locked);
    REQUIRE(mock.Calls().size() == 1u);
    REQUIRE(mock.Calls()[0] == hal::ActuatorTarget::Locked);
}

TEST_CASE("bolt: unlock after lock transitions to Unlocked")
{
    test::MockActuator mock;
    mock.SetNextResult(hal::ActuatorResult::Success);
    Bolt::Instance().Init(&mock);
    Bolt::Instance().Lock();

    REQUIRE(Bolt::Instance().Unlock());
    REQUIRE(Bolt::Instance().State() == LockState::Unlocked);
}

TEST_CASE("bolt: failed actuator leaves state Unknown")
{
    test::MockActuator mock;
    mock.SetNextResult(hal::ActuatorResult::Stalled);
    Bolt::Instance().Init(&mock);

    Bolt::Instance().Lock();
    REQUIRE(Bolt::Instance().State() == LockState::Unknown);
}
