/*
 * Copyright (c) 2025 openMatterSmartLock contributors
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Matter / CHIP project-specific configuration.
 */

#pragma once

// Device identification — Matter test vendor IDs for development
#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID                 0xFFF1
#define CHIP_DEVICE_CONFIG_DEVICE_PRODUCT_ID                0x8100
#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION          1
#define CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING   "v0.0.1"

// Door Lock cluster
#define CHIP_DEVICE_CONFIG_DEVICE_TYPE                      10

// Pairing
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_PIN_CODE          20202021
#define CHIP_DEVICE_CONFIG_USE_TEST_SETUP_DISCRIMINATOR     0xF00
