cmake_minimum_required(VERSION 3.10)
project(block_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(console)
add_subdirectory(gui)
