cmake_minimum_required(VERSION 3.16)

project(test-spintimer-cmocka)

# module under test
add_subdirectory(../../. spin-timer)

# stubs
add_subdirectory(../. spin-timer-test)

add_executable(${PROJECT_NAME} ${INCLUDE_DIRECTORIES} test.c cmocka/src/cmocka.c)
target_include_directories(${PROJECT_NAME} PUBLIC cmocka/include/. ../../.)
target_link_libraries(${PROJECT_NAME} spin-timer-test spin-timer)
