cmake_minimum_required(VERSION 3.15)
project(NetSuite VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Boost (required for serialization and shared_ptr)
find_package(Boost REQUIRED COMPONENTS serialization)

# Core library - pure business logic, no UI dependencies
add_library(netsuite_core STATIC
    # Base classes
    RT_Base.cpp
    RT_Base.h
    
    # Network engine
    RT_Network.cpp
    RT_Network.h
    
    # Cell classes
    RT_Cell.cpp
    RT_Cell.h
    RT_CurrentUser.cpp
    RT_CurrentUser.h
    
    # Current classes
    RT_Current.cpp
    RT_Current.h
    RT_HHCurrent.cpp
    RT_HHCurrent.h
    RT_HHKineticsFactor.cpp
    RT_HHKineticsFactor.h
    RT_HHLinearPiecewiseCurrent.cpp
    RT_HHLinearPiecewiseCurrent.h
    RT_HHLinearPiecewiseKineticsFactor.cpp
    RT_HHLinearPiecewiseKineticsFactor.h
    
    # Synapse classes
    RT_Synapse.cpp
    RT_Synapse.h
    
    # Electrode classes
    RT_Electrode.cpp
    RT_Electrode.h
    
    # Utilities
    RT_Utilities.h
    MonotCubicInterpolator.cpp
    MonotCubicInterpolator.h
    RARN.cpp
    RARN.h
    datalogger.h
    factory.h
)

target_include_directories(netsuite_core 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(netsuite_core 
    PUBLIC 
        Boost::serialization
)

# Define SERIALIZE for boost serialization
target_compile_definitions(netsuite_core PUBLIC SERIALIZE)

# Disable VCL-specific code
target_compile_definitions(netsuite_core PUBLIC NO_VCL)

# Optional: Build tests
option(BUILD_TESTS "Build unit tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Optional: Build Qt UI (will add later)
option(BUILD_QT_UI "Build Qt user interface" OFF)
if(BUILD_QT_UI)
    add_subdirectory(ui)
endif()

# Print configuration
message(STATUS "NetSuite Configuration:")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Boost version: ${Boost_VERSION}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build Qt UI: ${BUILD_QT_UI}")
