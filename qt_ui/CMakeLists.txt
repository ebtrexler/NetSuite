cmake_minimum_required(VERSION 3.15)

# Set Qt path
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Qt UI application
add_executable(netsuite_qt
    main.cpp
    mainwindow.cpp
    mainwindow.h
    hhcurrentdialog.cpp
    hhcurrentdialog.h
)

target_link_libraries(netsuite_qt
    netsuite_core
    Qt5::Widgets
)

target_include_directories(netsuite_qt PRIVATE
    ${CMAKE_SOURCE_DIR}
)
