cmake_minimum_required(VERSION 3.5)
project(cppqt_project_package)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(Boost 1.71.0 REQUIRED COMPONENTS thread)
find_package(Qt5 COMPONENTS Widgets Core Gui Network REQUIRED)

file(GLOB PRO_UI_DIR RELATIVE ${CMAKE_SOURCE_DIR} FOLLOW_SYMLINKS ui/*.ui)
file(GLOB PRO_INCLUDE_DIR RELATIVE ${CMAKE_SOURCE_DIR} FOLLOW_SYMLINKS include/${PROJECT_NAME}/*.h include/${PROJECT_NAME}/*.hpp)
file(GLOB PRO_RESOURCE_DIR RELATIVE ${CMAKE_SOURCE_DIR} FOLLOW_SYMLINKS resource/*.qrc)

qt5_wrap_cpp(QT_MOC_HPP ${PRO_INCLUDE_DIR})
qt5_add_resources(QT_RESOUCE_HPP ${PRO_RESOURCE_DIR})

include_directories(
  include/
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Chaser 노드 실행 파일
add_executable(chaser_node src/chaser_node.cpp src/grid_map.cpp)
ament_target_dependencies(chaser_node rclcpp geometry_msgs std_srvs)

# Player 노드 실행 파일
add_executable(player_node src/player_node.cpp src/grid_map.cpp)
ament_target_dependencies(player_node rclcpp geometry_msgs std_msgs std_srvs)

# GUI 노드 실행 파일
add_executable(game_gui_node
    src/main.cpp
    src/main_window.cpp
    src/grid_map.cpp
    src/grid_widget.cpp
    src/input_handler.cpp
    ${PRO_INCLUDE_DIR}
    ${PRO_UI_DIR}
    ${QT_MOC_HPP}
    ${QT_RESOUCE_HPP}
)
ament_target_dependencies(game_gui_node rclcpp geometry_msgs std_srvs)
target_link_libraries(game_gui_node Qt5::Widgets Qt5::Core Qt5::Gui)

install(TARGETS
  chaser_node
  player_node
  game_gui_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

set_target_properties(game_gui_node PROPERTIES
  AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

ament_package()