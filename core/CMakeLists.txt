project(${CMAKE_PROJECT_NAME}_core)

set(BLOCK_ENGINE_CORE_LIBRARY ${PROJECT_NAME} CACHE STRING "")

add_library(${BLOCK_ENGINE_CORE_LIBRARY}
    src/block.cpp
    src/bus_factory.cpp
    src/block_factory.cpp
)

message(STATUS "BLOCK_ENGINE_COMMON_LIBRARY: ${BLOCK_ENGINE_COMMON_LIBRARY}")
target_include_directories(${BLOCK_ENGINE_CORE_LIBRARY} PUBLIC include)
if (UNIX)
    target_link_libraries(${BLOCK_ENGINE_CORE_LIBRARY} PUBLIC ${BLOCK_ENGINE_COMMON_LIBRARY} pthread)
else()
    target_link_libraries(${BLOCK_ENGINE_CORE_LIBRARY} PUBLIC ${BLOCK_ENGINE_COMMON_LIBRARY})
endif (UNIX)
