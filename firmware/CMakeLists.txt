cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(openMatterSmartLock VERSION 0.0.1 LANGUAGES C CXX)

target_sources(app PRIVATE
    app/main.cpp
)

target_include_directories(app PRIVATE app)
