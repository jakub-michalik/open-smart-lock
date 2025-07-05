/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "runtime.h"

#include <zephyr/logging/log.h>

#include "adc_manager.h"
#include "lock/bolt.h"

LOG_MODULE_DECLARE(omsl);

namespace omsl {

constexpr int32_t kHeartbeatIntervalMs = 5000;

static AdcManager sAdc;

Runtime & Runtime::Instance()
{
    static Runtime instance;
    return instance;
}

int Runtime::StartApp()
{
    LOG_INF("Runtime::StartApp");

    Bolt::Instance().Init();
    sAdc.Init();

    k_work_init_delayable(&mHeartbeat, HeartbeatHandler);
    StartHeartbeat();

    while (true) {
        k_sleep(K_FOREVER);
    }
    return 0;
}

void Runtime::StartHeartbeat()
{
    k_work_schedule(&mHeartbeat, K_MSEC(kHeartbeatIntervalMs));
}

void Runtime::HeartbeatHandler(struct k_work * work)
{
    int32_t mv = sAdc.SampleBatteryMillivolts();
    LOG_INF("Heartbeat: battery=%d mV", mv);
    k_work_schedule(&Instance().mHeartbeat, K_MSEC(kHeartbeatIntervalMs));
}

}  // namespace omsl
