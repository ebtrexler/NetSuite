cmake_minimum_required(VERSION 3.16)
project(NetSuite_wx)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${Boost_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    main.cpp
    NetworkFrame.cpp
    CreateItemDialog.cpp
    ConnectionDialog.cpp
    JsonSerializer.cpp
    NetworkVisualizationPanel.cpp
    RT_Network_wx.cpp
    RT_Cell_wx.cpp
    RT_BiologicalCell_wx.cpp
    RT_Synapse_wx.cpp
    RT_Electrode_wx.cpp
)

# Create executable
add_executable(NetSuite_wx ${SOURCES})

# Link libraries
target_link_libraries(NetSuite_wx ${wxWidgets_LIBRARIES})
if(Boost_FOUND)
    target_link_libraries(NetSuite_wx ${Boost_LIBRARIES})
endif()

# Compiler definitions
target_compile_definitions(NetSuite_wx PRIVATE
    SERIALIZE
    SIM_ONLY_NO_NIDAQ
)