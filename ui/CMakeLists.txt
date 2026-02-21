cmake_minimum_required(VERSION 3.15)

# Set Qt path
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# NetSim application
add_executable(netsim
    ../apps/netsim/main.cpp
    main_window/mainwindow.cpp
    main_window/mainwindow.h
    visualizers/networkview.cpp
    visualizers/networkview.h
    visualizers/traceplot.cpp
    visualizers/traceplot.h
    visualizers/tracepanel.cpp
    visualizers/tracepanel.h
    editors/hhcurrentdialog.cpp
    editors/hhcurrentdialog.h
)

target_link_libraries(netsim
    netsuite_core
    Qt5::Widgets
)

target_include_directories(netsim PRIVATE
    ${CMAKE_SOURCE_DIR}/core/network
    ${CMAKE_SOURCE_DIR}/core/models
    ${CMAKE_SOURCE_DIR}/ui/main_window
    ${CMAKE_SOURCE_DIR}/ui/visualizers
    ${CMAKE_SOURCE_DIR}/ui/editors
)
