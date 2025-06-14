# CMake and C settings
cmake_minimum_required(VERSION 3.22)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
enable_testing()

# Set project name
project(time-travel)

# Collect source code files
file(GLOB_RECURSE SOURCES "src/*.c")
file(GLOB_RECURSE HEADERS "include/*.h")
set(TESTS "test/test.c" "src/finder.c" "include/finder.h")
include_directories(include)

# Make executables
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
add_executable(${PROJECT_NAME}-test ${TESTS})

# Link PCRE2 library
find_package(PkgConfig REQUIRED)
pkg_check_modules(PCRE2 REQUIRED libpcre2-8)
include_directories(${PCRE2_INCLUDE_DIRS})
link_directories(${PCRE2_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} ${PCRE2_LIBRARIES})
target_link_libraries(${PROJECT_NAME}-test ${PCRE2_LIBRARIES})

# Setup tests commands
add_test(NAME y-m-d COMMAND ${PROJECT_NAME}-test -d ../test/y-m-d)
add_test(NAME d-m-y COMMAND ${PROJECT_NAME}-test -d ../test/d-m-y)
add_test(NAME m-d COMMAND ${PROJECT_NAME}-test -d ../test/m-d)
add_test(NAME d-m COMMAND ${PROJECT_NAME}-test -d ../test/d-m)
