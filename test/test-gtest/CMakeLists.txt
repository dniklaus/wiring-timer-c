cmake_minimum_required(VERSION 3.16)

project(test-spintimer-gtest)

# module under test
add_subdirectory(../../. spin-timer)

# stubs and mocks
add_subdirectory(../. spin-timer-test)

add_subdirectory(gtest gtest)

add_executable(${PROJECT_NAME} ${INCLUDE_DIRECTORIES} test.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC . gtest/googletest/include gtest/googletest ../../.)
target_link_libraries(${PROJECT_NAME} spin-timer-test spin-timer gtest)
