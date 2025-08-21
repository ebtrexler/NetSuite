cmake_minimum_required(VERSION 3.16)
project(NetClamp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base html xml net)
include(${wxWidgets_USE_FILE})

# Find Boost (allow any version)
find_package(Boost COMPONENTS serialization)
if(NOT Boost_FOUND)
    message(STATUS "Boost not found, building without serialization support")
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../common
    ${Boost_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    main.cpp
    ../common/NetworkFrame.cpp
    ../common/CreateItemDialog.cpp
    ../common/ConnectionDialog.cpp
    ../common/CopyCurrentsDialog.cpp
    ../common/TextFileImportDialog.cpp
    ../common/JsonSerializer.cpp
    ../common/NetworkVisualizationPanel.cpp
    ../common/RT_Network_wx.cpp
    ../common/RT_Cell_wx.cpp
    ../common/RT_BiologicalCell_wx.cpp
    ../common/RT_Synapse_wx.cpp
    ../common/RT_Electrode_wx.cpp
    ../common/RT_Current_wx.cpp
)

# Create executable
add_executable(NetClamp ${SOURCES})

# Link libraries
target_link_libraries(NetClamp ${wxWidgets_LIBRARIES})
if(Boost_FOUND)
    target_link_libraries(NetClamp ${Boost_LIBRARIES})
endif()

# Compiler definitions
target_compile_definitions(NetClamp PRIVATE
    SERIALIZE
    ENABLE_DAQ
)