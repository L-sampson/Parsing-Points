cmake_minimum_required(VERSION 3.20)
project(ParsingPointsSever)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

# Fetch nlohmann/json header
FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
)
FetchContent_MakeAvailable(crow)

find_package(CURL REQUIRED)

# include all the header files
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_SOURCE_DIR}/logic
)
include_directories(${JSON_INCLUDE_DIRS})


file(GLOB_RECURSE data_src "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp")
file(GLOB_RECURSE logic_src "${CMAKE_CURRENT_SOURCE_DIR}/logic/*.cpp")

# Add source files
add_executable(
    API main.cpp 
    ${data_src}
    ${logic_src}
)

target_link_libraries(API PRIVATE CURL::libcurl Crow nlohmann_json::nlohmann_json)