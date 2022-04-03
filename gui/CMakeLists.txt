find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${CMAKE_PROJECT_NAME}_gui
    src/main.cpp
    src/qblockenginewindow.cpp
    src/qblockenginewindow.ui
    src/qschemeeditor.cpp
    include/qblockenginewindow.h
    include/qschemeeditor.h
)

target_include_directories(${CMAKE_PROJECT_NAME}_gui PUBLIC include)
target_link_libraries(${CMAKE_PROJECT_NAME}_gui Qt5::Widgets core)