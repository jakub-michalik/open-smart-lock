/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * ZCL Door Lock cluster callbacks. Bridges Matter cluster invocations
 * to the omsl::Bolt domain layer via the application
 * orchestrator.
 */

#include <app-common/zap-generated/cluster-objects.h>
#include <app/util/af.h>

#include <zephyr/logging/log.h>

LOG_MODULE_DECLARE(omsl);

// Door Lock cluster: LockDoor command
bool emberAfDoorLockClusterLockDoorCallback(
    chip::app::CommandHandler * commandObj,
    const chip::app::ConcreteCommandPath & commandPath,
    const chip::app::Clusters::DoorLock::Commands::LockDoor::DecodableType & commandData)
{
    LOG_INF("Door Lock: LockDoor");
    // TODO: route into Bolt
    return true;
}

// Door Lock cluster: UnlockDoor command
bool emberAfDoorLockClusterUnlockDoorCallback(
    chip::app::CommandHandler * commandObj,
    const chip::app::ConcreteCommandPath & commandPath,
    const chip::app::Clusters::DoorLock::Commands::UnlockDoor::DecodableType & commandData)
{
    LOG_INF("Door Lock: UnlockDoor");
    // TODO: route into Bolt
    return true;
}
