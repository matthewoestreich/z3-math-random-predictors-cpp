cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------------------------------------------------------#
# Set environment variables
#-------------------------------------------------------------------------------------------------------------------------------#

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wunused-variable -Wunused-parameter -Wall -Wextra")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(IS_64_BIT ${CMAKE_SIZEOF_VOID_P} EQUAL 8)

######################## These must be called before `project(...)` due to how cmake searches for files. ########################
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
set(VCPKG_BUILD_TYPE release)
# Explicitly set triplet after ensuring we are on a 64 bit system.
if(IS_64_BIT)
  if(APPLE)
    set(VCPKG_TARGET_TRIPLET x64-osx-release) # Set to release only for Mac (for some reason the dbg build takes forever on Mac).
  elseif(WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows)
  elseif(UNIX)
    set(VCPKG_TARGET_TRIPLET x64-linux)
  else()
    message(FATAL_ERROR "\nUNSUPPORTED OS\n")
  endif()
else()
  message(FATAL_ERROR "\nUNSUPPORTED ARCHITECTURE\n")
endif()
#################### end These must be called before `project(...)` due to how cmake searches for files. ########################

#-------------------------------------------------------------------------------------------------------------------------------#
# Create project
#-------------------------------------------------------------------------------------------------------------------------------#

project(js-randomness-predictor)

# Globally include headers for every target
include_directories(include)

#-------------------------------------------------------------------------------------------------------------------------------#
# Add JSRandomnessPredictor library
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
# Enable testing and include '/tests/CMakeLists.txt' for testing binaries
#-------------------------------------------------------------------------------------------------------------------------------#

# Must be in the root CMakeLists.txt
enable_testing()

include(CTest)

# Must be in the root CMakeLists.txt
set(CMAKE_CTEST_ARGUMENTS "--output-on-failure" "--verbose")

add_subdirectory(tests)

#-------------------------------------------------------------------------------------------------------------------------------#
# Custom commands
#-------------------------------------------------------------------------------------------------------------------------------#

add_custom_target(fmt
  COMMAND clang-format
    -i
    -style=file
    --fallback-style=none
    --assume-filename=$ENV{HOME}/.clang-format
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
    ${PROJECT_SOURCE_DIR}/include/*.hpp
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)