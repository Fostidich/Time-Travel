cmake_minimum_required(VERSION 3.22)

project(time-travel)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.c" "lib/*.c")
file(GLOB_RECURSE HEADERS "include/*.h" "lib/*.h")
file(GLOB_RECURSE TESTS "test/*")

include_directories(include lib)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

add_executable(${PROJECT_NAME}-test ${SOURCES} ${HEADERS} ${TESTS})
target_compile_definitions(${PROJECT_NAME}-test PRIVATE TEST)

