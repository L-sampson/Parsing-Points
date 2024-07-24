cmake_minimum_required(VERSION 3.20)
project(ParsingPointsSever)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch Curl library
# FetchContent_Declare(
#     curl
#     GIT_REPOSITORY https://github.com/curl/curl.git
# )
# FetchContent_MakeAvailable(curl)

# Fetch nlohmann/json header
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(json)

find_package(CURL REQUIRED)

# include all the header files
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_SOURCE_DIR}/logic
)

file(GLOB_RECURSE data_src "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp")
file(GLOB_RECURSE logic_src "${CMAKE_CURRENT_SOURCE_DIR}/logic/*.cpp")

# Add source files
add_executable(
    API main.cpp 
    ${data_src}
    ${logic_src}
)

target_link_libraries(API PRIVATE CURL::libcurl)