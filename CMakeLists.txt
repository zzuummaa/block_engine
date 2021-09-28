cmake_minimum_required(VERSION 3.19)
project(block_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(block_engine src/link_engine.cpp include/type_info.h src/connector.cpp)
target_include_directories(block_engine PUBLIC include)
target_link_libraries(block_engine INTERFACE nlohmann_json::nlohmann_json)

add_executable(example test/main.cpp)
target_link_libraries(example PRIVATE block_engine)
