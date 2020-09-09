cmake_minimum_required(VERSION 3.16)
project(spin-timer-test)

add_library(${PROJECT_NAME} STATIC
    MockTimerAdapter.c
    StubTestUptimeInfo.c)
target_include_directories(${PROJECT_NAME} PUBLIC . )
