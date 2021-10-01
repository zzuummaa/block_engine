project(tests)

add_executable(test main.cpp)
target_link_libraries(test PRIVATE block_engine)

add_executable(test_json main_json.cpp)
target_link_libraries(test_json PRIVATE block_engine)
