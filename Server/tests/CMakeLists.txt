cmake_minimum_required(VERSION 3.25)
project(ServerMock)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.0
)
FetchContent_MakeAvailable(googletest)


set(DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../data)
include_directories(${DATA_DIR})
file(GLOB option_url_src "${DATA_DIR}/option_url.cpp")

message("Current list of source files:")
foreach(source_file ${option_url_src})
    message("${source_file}")
endforeach()

add_executable(server_mock server_mock_test.cpp ${option_url_src})
target_link_libraries(server_mock PRIVATE gtest_main gmock_main)

enable_testing()
add_test(NAME ServerMockTest COMMAND server_mock)