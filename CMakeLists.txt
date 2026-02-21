cmake_minimum_required(VERSION 3.15)
project(NetSuite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost (required for serialization)
find_package(Boost REQUIRED COMPONENTS serialization)

# Core library - pure business logic, no UI dependencies
add_library(netsuite_core STATIC
    # Network classes
    core/network/RT_Base.cpp
    core/network/RT_Base.h
    core/network/RT_Network.cpp
    core/network/RT_Network.h
    core/network/RT_Cell.cpp
    core/network/RT_Cell.h
    core/network/RT_CurrentUser.cpp
    core/network/RT_CurrentUser.h
    core/network/RT_Current.cpp
    core/network/RT_Current.h
    core/network/RT_Synapse.cpp
    core/network/RT_Synapse.h
    core/network/RT_Electrode.cpp
    core/network/RT_Electrode.h
    
    # Model classes
    core/models/RT_HHCurrent.cpp
    core/models/RT_HHCurrent.h
    core/models/RT_HHKineticsFactor.cpp
    core/models/RT_HHKineticsFactor.h
    core/models/RT_HHLinearPiecewiseCurrent.cpp
    core/models/RT_HHLinearPiecewiseCurrent.h
    core/models/RT_HHLinearPiecewiseKineticsFactor.cpp
    core/models/RT_HHLinearPiecewiseKineticsFactor.h
    
    # Math utilities
    core/math/MonotCubicInterpolator.cpp
    core/math/MonotCubicInterpolator.h
    core/math/RARN.cpp
    core/math/RARN.h
    core/math/RT_Utilities.h
    
    # IO utilities
    core/io/datalogger.h
    core/io/factory.h
)

target_include_directories(netsuite_core 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/core/network
        ${CMAKE_CURRENT_SOURCE_DIR}/core/models
        ${CMAKE_CURRENT_SOURCE_DIR}/core/math
        ${CMAKE_CURRENT_SOURCE_DIR}/core/io
)

target_link_libraries(netsuite_core 
    PUBLIC 
        Boost::serialization
)

target_compile_definitions(netsuite_core PUBLIC NO_VCL SERIALIZE)

# Test executable
add_executable(test_core apps/test_core.cpp)
target_link_libraries(test_core netsuite_core)

# Qt UI
option(BUILD_QT_UI "Build Qt user interface" ON)
if(BUILD_QT_UI)
    add_subdirectory(ui)
endif()

# Print configuration
message(STATUS "NetSuite Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Boost version: ${Boost_VERSION}")
message(STATUS "  Build Qt UI: ${BUILD_QT_UI}")
