#
# Copyright (c) 2025 openMatterSmartLock contributors
#
# SPDX-License-Identifier: Apache-2.0
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(openMatterSmartLock VERSION 0.1.0 LANGUAGES C CXX)

# GNU C++ extensions required by the Matter stack
include(${ZEPHYR_CONNECTEDHOMEIP_MODULE_DIR}/config/nrfconnect/app/enable-gnu-std.cmake)

# Shared NCS Matter sample-common infrastructure
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
    app/matter/zcl_callbacks.cpp
    app/lock/bolt.cpp
    app/lock/pin.cpp
    app/access/types.cpp
    app/access/roster.cpp
    app/access/users.cpp
    app/access/credentials.cpp
    app/access/vault.cpp
    app/ui/console.cpp
)

if(CONFIG_LOCK_SCHEDULES)
    target_sources(app PRIVATE app/access/schedules.cpp)
endif()

# HAL driver subdirectories — each gated by its own Kconfig
add_subdirectory(drivers/actuator/servo_pwm)
add_subdirectory(drivers/feedback/adc_potentiometer)
add_subdirectory(drivers/power/gpio_gate)

# Configure the Matter data model (Door Lock cluster + dependencies).
# Suppress the false-positive "maybe-uninitialized" warning in the upstream
# door-lock-server when compiled by the current toolchain.
target_compile_options(matter-data-model PRIVATE -Wno-error=maybe-uninitialized)

ncs_configure_data_model()

# All driver source files are added by driver subdirs above; nothing here
# should include drivers/* directly.
