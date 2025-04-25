cmake_minimum_required(VERSION 3.10)

# Must be called before `project(...)` due to how cmake searches for files.
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_BUILD_TYPE release)

project(js-randomness-predictor)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(THIRD_PARTY_LIBS_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/nix")

#-------------------------------------------------------------------------------------------------------------------------------#
# Main
#-------------------------------------------------------------------------------------------------------------------------------#

find_package(Z3 CONFIG REQUIRED)

file(GLOB_RECURSE CPP_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(main ${CPP_SOURCES})

target_include_directories(main
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(main 
  PRIVATE
    z3::libz3
    #${THIRD_PARTY_LIBS}
)


#-------------------------------------------------------------------------------------------------------------------------------#
# Tests
#-------------------------------------------------------------------------------------------------------------------------------#

find_package(Catch2 CONFIG REQUIRED)
find_package(Z3 CONFIG REQUIRED)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

# Filter out main.cpp before passing to tests.
# This is huge! Otherwise Catch2 will silently fail!!
# Can't have a main method + `#define CATCH_CONFIG_MAIN`.
list(FILTER CPP_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_executable(tests ${TEST_SOURCES} ${CPP_SOURCES})

target_include_directories(tests 
  PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(tests 
  PRIVATE 
    Catch2::Catch2WithMain
    z3::libz3
)

# ctest/Catch2 specific
enable_testing()

set(CMAKE_CTEST_ARGUMENTS "--verbose" "--output-on-failure")

add_test(
  NAME tests 
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tests --reporter console --colour-mode ansi -s
)
# end ctest/Catch2 specific


#-------------------------------------------------------------------------------------------------------------------------------#
# Commands
#-------------------------------------------------------------------------------------------------------------------------------#

add_custom_target(run
    COMMAND ./main
    DEPENDS main
)