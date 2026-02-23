cmake_minimum_required(VERSION 3.15)

# Set Qt path
set(CMAKE_PREFIX_PATH "/opt/homebrew/opt/qt@5")

# Find Qt5
find_package(Qt5 REQUIRED COMPONENTS Widgets)

# Enable automoc for Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# NETrex application
set(NETREX_ICON_SOURCES "")
if(WIN32)
    set(NETREX_ICON_SOURCES ${CMAKE_SOURCE_DIR}/resources/NETrex.rc)
elseif(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE NETrex.icns)
    set(NETREX_ICNS ${CMAKE_SOURCE_DIR}/resources/NETrex.icns)
    set_source_files_properties(${NETREX_ICNS} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(NETREX_ICON_SOURCES ${NETREX_ICNS})
endif()

add_executable(NETrex
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
    editors/networkeditor.cpp
    editors/networkeditor.h
    editors/modelcelldialog.h
    editors/electrodedialog.h
    editors/synapsedialog.h
    editors/hh2currentdialog.h
    editors/voltageclamppiddialog.h
    editors/playbackwaveformdialog.h
    editors/playbackcurrentdialog.h
    editors/playbackcelldialog.h
    editors/playbackelectrodedialog.h
    editors/biologicalcelldialog.h
    editors/rundialog.h
    ${NETREX_ICON_SOURCES}
    resources.qrc
)

target_link_libraries(NETrex
    netsuite_core
    Qt5::Widgets
)

target_include_directories(NETrex PRIVATE
    ${CMAKE_SOURCE_DIR}/core/network
    ${CMAKE_SOURCE_DIR}/core/models
    ${CMAKE_SOURCE_DIR}/core/math
    ${CMAKE_SOURCE_DIR}/core/io
    ${CMAKE_SOURCE_DIR}/core/daq
    ${CMAKE_SOURCE_DIR}/ui/main_window
    ${CMAKE_SOURCE_DIR}/ui/visualizers
    ${CMAKE_SOURCE_DIR}/ui/editors
)
