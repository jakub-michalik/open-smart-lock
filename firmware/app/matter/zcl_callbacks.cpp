/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Matter Door Lock cluster callbacks. Bridges cluster invocations from
 * the Matter stack to the omsl::Bolt domain layer.
 */

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <app/clusters/door-lock-server/door-lock-server.h>
#include <app/data-model/Nullable.h>

#include <zephyr/logging/log.h>

#include "lock/bolt.h"

LOG_MODULE_DECLARE(omsl);

using namespace ::chip;
using namespace ::chip::app::Clusters;
using namespace ::chip::app::Clusters::DoorLock;
using ::chip::app::DataModel::Nullable;

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & path, uint8_t type,
                                       uint16_t size, uint8_t * value)
{
    // No-op for now: Door Lock cluster attribute changes are routed
    // back into the Bolt domain through the dedicated lock/unlock hooks.
}

bool emberAfPluginDoorLockOnDoorLockCommand(chip::EndpointId endpointId,
                                            const Nullable<chip::FabricIndex> & fabricIdx,
                                            const Nullable<chip::NodeId> & nodeId,
                                            const Optional<chip::ByteSpan> & pinCode,
                                            OperationErrorEnum & err)
{
    LOG_INF("Door Lock: LockDoor (Matter)");
    return omsl::Bolt::Instance().Lock();
}

bool emberAfPluginDoorLockOnDoorUnlockCommand(chip::EndpointId endpointId,
                                              const Nullable<chip::FabricIndex> & fabricIdx,
                                              const Nullable<chip::NodeId> & nodeId,
                                              const Optional<chip::ByteSpan> & pinCode,
                                              OperationErrorEnum & err)
{
    LOG_INF("Door Lock: UnlockDoor (Matter)");
    return omsl::Bolt::Instance().Unlock();
}
