# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(V8Predictor)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set optimization flags and include directories
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -I./include")

# Include the Z3 library
set(Z3_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/libz3.a")

# Specify the source files
file(GLOB SRCS "*.cpp")

# Add the executable target
add_executable(main ${SRCS})

# Link the Z3 library to the target
target_link_libraries(main ${Z3_LIB_PATH})

# Custom clean target (renamed to avoid reserved "clean" target name)
add_custom_target(clean_build
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_clean.cmake
)

# Custom run target
add_custom_target(run
    COMMAND ./main
    DEPENDS main
)
