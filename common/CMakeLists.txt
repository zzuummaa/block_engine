project(common)

add_library(${PROJECT_NAME}
        src/scheme.cpp
        src/bus.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)