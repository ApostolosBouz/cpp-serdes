cmake_minimum_required(VERSION 3.28)

project(cpp_serdes
    VERSION 0.0.1
    LANGUAGES CXX
    DESCRIPTION "Messing around with basic stl serialisation - deserialisation")

if(PROJECT_IS_TOP_LEVEL)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib/$<CONFIG>")
endif()

include(GNUInstallDirs)

# Enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets")

# Sources
set(cpp_serdes_entry "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

## Executable
add_executable(cpp_serdes ${cpp_serdes_entry})

# Source and Include directories
target_sources(cpp_serdes PRIVATE ${cpp_serdes_entry})

set_target_properties(cpp_serdes PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

if(MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT cpp_serdes)
    set_target_properties(cpp_serdes PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:cpp_serdes>")
endif()

# Setting IDE tree structure
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/" PREFIX "source" FILES ${cpp_serdes_entry})
