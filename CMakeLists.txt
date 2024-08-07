cmake_minimum_required(VERSION 3.22)

project(time-travel)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
enable_testing()

file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")
set(TESTS "test/main.c" "src/finder.c" "include/finder.h")
include_directories(include)

# Make executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Make tests executable
add_executable(${PROJECT_NAME}-test ${TESTS})

# Link PCRE2 library
link_directories("/usr/lib/x86_64-linux-gnu")
target_link_libraries(${PROJECT_NAME} pcre2-8)
target_link_libraries(${PROJECT_NAME}-test pcre2-8)

# Setup tests command
set(FOUND 0)
set(UNSURE 1)
set(UNKNOWN 2)
set(UNCHANGED 3)
set(FAILURE 4)

add_test(NAME 2024-08-07 COMMAND ${PROJECT_NAME}-test 2024-08-07 2024-08-07 ${FOUND})
