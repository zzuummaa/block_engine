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
    src/camera_controller.cpp
    src/scene_controller.cpp
    src/scheme_editor_model.cpp
    src/qschemeeditor.cpp
    src/qpin.cpp
    src/pin_group_holder.cpp
    src/qbusline.cpp
    src/qbus.cpp
    src/qblock.cpp
    src/qblock.ui
    src/qblocklist.cpp
    include/qblockenginewindow.h
    include/scheme_editor/camera_controller.h
    include/scheme_editor/scene_controller.h
    include/scheme_editor/scheme_editor_model.h
    include/scheme_editor/qschemeeditor.h
    include/qpin.h
    include/pin_group_holder.h
    include/qbusline.h
    include/qbus.h
    include/qblock.h
    include/qblocklist.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${CMAKE_PROJECT_NAME}_core)