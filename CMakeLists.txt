cmake_minimum_required(VERSION 3.22)

project(time-travel)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.c" "lib/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "lib/*.h")
include_directories(include lib)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_executable(tt-test test/findertest.c)
add_test(NAME finder_test COMMAND tt-test)

set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../target)
