find_package(Catch2 CONFIG REQUIRED)

include(Catch)

file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(test-runner ${TEST_SOURCES})

target_link_libraries(test-runner 
  PRIVATE 
    Catch2::Catch2WithMain
    JSRandomnessPredictor
)

catch_discover_tests(test-runner
  REPORTER console
  EXTRA_ARGS --colour-mode ansi -s
)