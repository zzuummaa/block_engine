project(core)

add_library(${PROJECT_NAME}
        src/bus_factory.cpp
        src/block_factory.cpp
        src/connector.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC common)

add_executable(${PROJECT_NAME}_test
        test/core_test.cpp)
target_link_libraries(${PROJECT_NAME}_test PUBLIC ${PROJECT_NAME})
