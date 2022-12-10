project(${CMAKE_PROJECT_NAME}_gui)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/block_type_info.cpp
    src/block_factory.cpp
    src/qblockenginewindow.cpp
    src/qblockenginewindow.ui
    src/qschemeeditor.cpp
    src/qblock.cpp
    src/qblocklist.cpp
    include/qblockenginewindow.h
    include/qschemeeditor.h
    include/qblock.h
    include/qblocklist.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${CMAKE_PROJECT_NAME}_core)