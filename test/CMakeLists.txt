project(tests)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE block_engine)

add_executable(test_json main_json.cpp)
target_link_libraries(test_json PRIVATE block_engine core)
