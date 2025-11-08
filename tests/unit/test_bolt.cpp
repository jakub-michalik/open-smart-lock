/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>

#include "lock/bolt.h"
#include "mocks/mock_actuator.h"

using namespace omsl;

ZTEST_SUITE(bolt, NULL, NULL, NULL, NULL, NULL);

ZTEST(bolt, init_with_null_actuator_handles_gracefully)
{
    Bolt::Instance().Init(nullptr);
    zassert_false(Bolt::Instance().Lock());
}

ZTEST(bolt, lock_transitions_to_locked_on_success)
{
    test::MockActuator mock;
    mock.SetNextResult(hal::ActuatorResult::Success);
    Bolt::Instance().Init(&mock);

    zassert_true(Bolt::Instance().Lock());
    zassert_equal(Bolt::Instance().State(), LockState::Locked);
    zassert_equal(mock.Calls().size(), 1u);
    zassert_equal(mock.Calls()[0], hal::ActuatorTarget::Locked);
}

ZTEST(bolt, unlock_after_lock_transitions_correctly)
{
    test::MockActuator mock;
    Bolt::Instance().Init(&mock);
    Bolt::Instance().Lock();

    zassert_true(Bolt::Instance().Unlock());
    zassert_equal(Bolt::Instance().State(), LockState::Unlocked);
}

ZTEST(bolt, failed_actuator_leaves_state_unknown)
{
    test::MockActuator mock;
    mock.SetNextResult(hal::ActuatorResult::Stalled);
    Bolt::Instance().Init(&mock);

    Bolt::Instance().Lock();
    zassert_equal(Bolt::Instance().State(), LockState::Unknown);
}
