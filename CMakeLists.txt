cmake_minimum_required(VERSION 3.10)
project(Infinitas CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
    message(STATUS "Building with debug mode.")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstack-protector")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMPXX REQUIRED IMPORTED_TARGET gmpxx)

enable_testing()

add_subdirectory(shared)
add_subdirectory(virtual-machine)
add_subdirectory(compiler)
add_subdirectory(parser)
add_subdirectory(lexer)
add_subdirectory(infinitas)

add_subdirectory(vendor/spdlog)
add_subdirectory(vendor/multiprecision-boost)

#add_subdirectory(tests)
