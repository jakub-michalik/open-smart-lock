/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Project-specific overrides to the default CHIP / Matter configuration.
 * Device identification values (vendor ID, product ID, software version,
 * device type) come from Kconfig options (CONFIG_CHIP_DEVICE_*) and must
 * NOT be redefined here — the NCS platform config derives compile-time
 * macros from Kconfig automatically.
 */

#pragma once

#ifdef CONFIG_THREAD_WIFI_SWITCHING
/* On the dual-transport variant where flash budget is tight, drop ZCL
 * progress logs (Door Lock cluster is the dominant contributor). */
#define CHIP_CONFIG_LOG_MODULE_Zcl_PROGRESS 0

/* Do not auto-register the Thread Network Commissioning instance when
 * both transports coexist; the application picks one explicitly. */
#define _NO_NETWORK_COMMISSIONING_DRIVER_
#endif
