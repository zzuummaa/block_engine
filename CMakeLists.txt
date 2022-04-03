cmake_minimum_required(VERSION 3.10)
project(block_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(gui)

add_library(block_engine
    src/link_logic.cpp
    common/src/scheme.cpp)

target_include_directories(block_engine PUBLIC include)
target_link_libraries(block_engine PUBLIC common nlohmann_json::nlohmann_json)

add_subdirectory(test)