cmake_minimum_required(VERSION 3.10)
project(JSRandomnessPredictor)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Gather source files recursively
file(GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/src/*.cpp")
add_executable(main ${SRCS})

# Determine lib subfolder based on OS and architecture
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message(STATUS "Detected macOS")
    set(LIB_PLATFORM_DIR "${CMAKE_SOURCE_DIR}/lib/nix")
    file(GLOB PLATFORM_LIBS "${LIB_PLATFORM_DIR}/*.a")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    message(STATUS "Detected Linux")
    set(LIB_PLATFORM_DIR "${CMAKE_SOURCE_DIR}/lib/nix")
    file(GLOB PLATFORM_LIBS "${LIB_PLATFORM_DIR}/*.a")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    message(STATUS "Detected Windows")
    set(LIB_PLATFORM_DIR "${CMAKE_SOURCE_DIR}/lib/windows")
    file(GLOB PLATFORM_LIBS "${LIB_PLATFORM_DIR}/*.lib" "${LIB_PLATFORM_DIR}/*.dll")
else()
    message(FATAL_ERROR "Unsupported OS: ${CMAKE_SYSTEM_NAME}")
endif()

# Add library path and automatically link the libraries
link_directories(${LIB_PLATFORM_DIR})
target_link_libraries(main ${PLATFORM_LIBS})

# Custom targets
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
)

add_custom_target(run
    COMMAND ./main
    DEPENDS main
)
