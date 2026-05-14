# Copyright (c) 2026 openMatterSmartLock contributors
# SPDX-License-Identifier: Apache-2.0
#
# Host-side unit tests for the openMatterSmartLock domain layer.
# Compiles the Bolt state machine and Roster access management against
# a thin Zephyr-logging shim (zephyr_shim/) so the same source files
# that ship on-target can be exercised on a developer host or in CI
# without a Zephyr toolchain.

cmake_minimum_required(VERSION 3.20)

# doctest 2.4.11 still declares cmake_minimum_required(VERSION 2.8.x), which
# trips CMake 4's compatibility check. Allow the older policy for nested
# subprojects only.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5 CACHE STRING "" FORCE)

project(omsl_host_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
endif()

include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.11
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(doctest)

set(FW_APP ${CMAKE_CURRENT_SOURCE_DIR}/../../firmware/app)
set(UNIT_MOCKS ${CMAKE_CURRENT_SOURCE_DIR}/../unit/mocks)

add_executable(omsl_host_tests
    test_main.cpp
    test_bolt.cpp
    test_roster.cpp
    ${FW_APP}/lock/bolt.cpp
    ${FW_APP}/access/roster.cpp
    ${FW_APP}/access/users.cpp
    ${FW_APP}/access/credentials.cpp
    ${FW_APP}/access/schedules.cpp
)

target_include_directories(omsl_host_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/zephyr_shim
    ${FW_APP}
    ${UNIT_MOCKS}/..  # so "mocks/mock_actuator.h" resolves
)

target_link_libraries(omsl_host_tests PRIVATE doctest::doctest)

enable_testing()
include(${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake)
doctest_discover_tests(omsl_host_tests)
