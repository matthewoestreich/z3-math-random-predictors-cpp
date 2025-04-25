cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------------------------------------------------------#
# Set Environment Variables
#-------------------------------------------------------------------------------------------------------------------------------#

# ~~ These must be called before `project(...)` due to how cmake searches for files. ~~
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_BUILD_TYPE release)
## Only set this if on Mac (for some reason the dbg build takes forever on Mac).
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(VCPKG_TARGET_TRIPLET x64-osx-release)
endif()
# ~~ end # These must be called before `project(...)` due to how cmake searches for files. ~~

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#-------------------------------------------------------------------------------------------------------------------------------#
# Create Project
#-------------------------------------------------------------------------------------------------------------------------------#

project(js-randomness-predictor)

#-------------------------------------------------------------------------------------------------------------------------------#
# Add JSRandomnessPredictor Library
#-------------------------------------------------------------------------------------------------------------------------------#

find_package(Z3 CONFIG REQUIRED)

file(GLOB_RECURSE CPP_SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_library(JSRandomnessPredictor STATIC ${CPP_SOURCES})

target_include_directories(JSRandomnessPredictor
  PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(JSRandomnessPredictor 
  PUBLIC
    z3::libz3
)

#-------------------------------------------------------------------------------------------------------------------------------#
# Include '/tests/CMakeLists.txt' for Testing Binaries
#-------------------------------------------------------------------------------------------------------------------------------#

# Must be in the root CMakeLists.txt
enable_testing()

include(CTest)

# Must be in the root CMakeLists.txt
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure" "--verbose")

add_subdirectory(tests)

#-------------------------------------------------------------------------------------------------------------------------------#
# Custom Commands
#-------------------------------------------------------------------------------------------------------------------------------#

# Add custom commands here.