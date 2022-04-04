project(${CMAKE_PROJECT_NAME}_common)

set(BLOCK_ENGINE_COMMON_LIBRARY ${PROJECT_NAME} CACHE STRING "")

add_library(${BLOCK_ENGINE_COMMON_LIBRARY}
    src/scheme.cpp
    src/bus.cpp
)
target_include_directories(${BLOCK_ENGINE_COMMON_LIBRARY} PUBLIC include)
target_link_libraries(${BLOCK_ENGINE_COMMON_LIBRARY} PUBLIC nlohmann_json::nlohmann_json)