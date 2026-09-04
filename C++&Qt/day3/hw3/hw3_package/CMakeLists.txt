cmake_minimum_required(VERSION 3.19)

project(hw2_package LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt6 COMPONENTS Widgets QUIET)

if(Qt6_FOUND)
    set(QT_LIB Qt6::Widgets)
else()
    find_package(Qt5 REQUIRED
COMPONENTS Widgets)
    set(QT_LIB Qt5::Widgets)
endif()

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/mainwindow.cpp
    include/mainwindow.h
    ui/mainwindow.ui
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${QT_LIB}
)
