find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${CMAKE_PROJECT_NAME}_gui
    src/main.cpp
    src/qblockenginewindow.cpp
    src/qblockenginewindow.ui
    src/scheme_editor/qschemeeditor.cpp
    include/qblockenginewindow.h
    include/scheme_editor/qschemeeditor.h
)

target_include_directories(${CMAKE_PROJECT_NAME}_gui PUBLIC include)
target_link_libraries(${CMAKE_PROJECT_NAME}_gui Qt5::Widgets)