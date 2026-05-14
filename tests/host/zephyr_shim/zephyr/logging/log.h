/*
 * Copyright (c) 2026 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Host-test shim for <zephyr/logging/log.h>. The domain layer uses
 * Zephyr's logging macros for diagnostic output; in host tests we
 * compile them out to no-ops so the domain sources can link without
 * a Zephyr toolchain. This file is only on the include path when
 * building under tests/host/.
 */

#pragma once

#define LOG_MODULE_REGISTER(...) static const int _omsl_log_unused = 0
#define LOG_MODULE_DECLARE(...)  static const int _omsl_log_unused = 0

#define LOG_INF(...) ((void) 0)
#define LOG_WRN(...) ((void) 0)
#define LOG_ERR(...) ((void) 0)
#define LOG_DBG(...) ((void) 0)
