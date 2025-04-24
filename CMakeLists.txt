cmake_minimum_required(VERSION 3.10)
project(JSRandomnessPredictor)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/include
)

file(GLOB SRCS "${CMAKE_SOURCE_DIR}/**/*.cpp")
set_property(GLOBAL PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(main ${SRCS})

link_directories(${CMAKE_SOURCE_DIR}/lib)
file(GLOB LIBRARIES ${CMAKE_SOURCE_DIR}/lib/lib*.a)
target_link_libraries(main ${LIBRARIES})

add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
)

add_custom_target(run
    COMMAND ./main
    DEPENDS main
)
