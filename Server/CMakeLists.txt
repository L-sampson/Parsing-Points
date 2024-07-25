cmake_minimum_required(VERSION 3.20)
project(ParsingPointsSever)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/data 
    ${CMAKE_CURRENT_SOURCE_DIR}/logic
    ${JSON_INCLUDE_DIRS}
)

# Link source files
file(GLOB data_src "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp")
file(GLOB logic_src "${CMAKE_CURRENT_SOURCE_DIR}/logic/*.cpp")

# Add source files
add_executable(
    server main.cpp 
    ${data_src}
    ${logic_src}
)

target_link_libraries(server PRIVATE CURL::libcurl Crow nlohmann_json::nlohmann_json)

add_subdirectory(tests)