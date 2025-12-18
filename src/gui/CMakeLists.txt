# CMakeLists.txt for Qt GUI module

cmake_minimum_required(VERSION 3.16)
project(coreMusicPlayerGUI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

# Set source files
set(SOURCES
    src/gui/main_window.cpp
    src/gui/main_window.ui
)

set(HEADERS
    src/gui/main_window.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME} 
    Qt6::Core 
    Qt6::Widgets 
    Qt6::Gui
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)