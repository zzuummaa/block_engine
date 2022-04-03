project(${CMAKE_PROJECT_NAME}_common)

add_library(${PROJECT_NAME}
        src/scheme.cpp
        src/bus.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json)