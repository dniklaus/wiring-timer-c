cmake_minimum_required(VERSION 3.16)
project(spin-timer)

add_library(${PROJECT_NAME} STATIC SpinTimer.c)
target_include_directories(${PROJECT_NAME} PUBLIC . )