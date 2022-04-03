project(${CMAKE_PROJECT_NAME}_console)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_PROJECT_NAME}_core)