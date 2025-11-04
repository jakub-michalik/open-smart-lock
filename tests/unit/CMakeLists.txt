cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(omsl_unit_tests LANGUAGES C CXX)

target_sources(app PRIVATE
    test_bolt.cpp
    test_roster.cpp
    ../../firmware/app/lock/bolt.cpp
    ../../firmware/app/access/roster.cpp
    ../../firmware/app/access/types.cpp
    ../../firmware/app/access/users.cpp
    ../../firmware/app/access/credentials.cpp
)

target_include_directories(app PRIVATE
    ../../firmware/app
    mocks
)
