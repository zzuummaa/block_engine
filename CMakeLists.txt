cmake_minimum_required(VERSION 3.10)
project(block_engine)

option(BLOCK_ENGINE_SANITIZERS_ENABLE "" OFF)

IF (WIN32)
    set(CMAKE_CXX_STANDARD 17)
ELSE()
    set(CMAKE_CXX_STANDARD 20)
ENDIF()

find_package(nlohmann_json 3.2.0 REQUIRED)

add_compile_options(-Wall -Wextra -pedantic -Werror)
IF (WIN32)
    add_compile_options(-Wno-error=c++20-designator)
ELSE()
    add_compile_options(-Wno-error=missing-field-initializers -Wno-attributes)
ENDIF()

IF (BLOCK_ENGINE_SANITIZERS_ENABLE)
#    set(SANITIZERS_OPTIONS "address,undefined,leak")
    set(SANITIZERS_OPTIONS "thread")
    message(STATUS "Sanitizers: ${SANITIZERS_OPTIONS}")
    add_compile_options(-fno-omit-frame-pointer -fsanitize=${SANITIZERS_OPTIONS})
    add_link_options(-fsanitize=${SANITIZERS_OPTIONS})
ENDIF()

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(console)
add_subdirectory(gui)
