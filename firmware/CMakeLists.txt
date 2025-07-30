#
# Copyright (c) 2025 openMatterSmartLock contributors
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(openMatterSmartLock VERSION 0.0.1 LANGUAGES C CXX)

# Enable GNU C++ extensions where the Matter stack needs them.
include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)

# Pull in shared Matter sample-common sources, data model helpers, and the
# ZAP integration glue. These come from the NCS Matter samples common
# directory; openMatterSmartLock is structured as an NCS Matter application
# and benefits from this shared infrastructure rather than re-implementing it.
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/source_common.cmake)
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/data_model.cmake)
include(${ZEPHYR_NRF_MODULE_DIR}/samples/matter/common/cmake/zap_helpers.cmake)

ncs_get_zap_parent_dir(ZAP_PARENT_DIR)

target_include_directories(app PRIVATE BEFORE app)
target_include_directories(app PRIVATE ${ZAP_PARENT_DIR})

# Application sources
target_sources(app PRIVATE
    app/main.cpp
    app/runtime.cpp
)

# Configure the Matter data model (Door Lock cluster + dependents).
ncs_configure_data_model()

target_sources(app PRIVATE
    app/matter/zcl_callbacks.cpp
)

target_sources(app PRIVATE
    app/lock/bolt.cpp
)

target_sources(app PRIVATE
    app/servo_controller.cpp
)

target_sources(app PRIVATE
    app/adc_manager.cpp
)

target_sources(app PRIVATE
    app/access/types.cpp
    app/access/roster.cpp
    app/access/users.cpp
    app/access/credentials.cpp
    app/access/vault.cpp
)
