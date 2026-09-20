cmake_minimum_required(VERSION 3.8)
project(vision_day1_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(Qt5 COMPONENTS Widgets Core Gui Network Multimedia MultimediaWidgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(cv_bridge REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Qt settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}
)

# 1. camera_node 실행 파일
add_executable(camera_node
  src/camera_node/camera_node.cpp
  include/${PROJECT_NAME}/camera_node/camera_node.hpp
)
target_include_directories(camera_node PRIVATE include)
ament_target_dependencies(camera_node rclcpp std_msgs sensor_msgs OpenCV cv_bridge)

# 2. ui_node 실행 파일 (독립된 ROS Node)
add_executable(ui_node
  src/ui_node/ui_node.cpp
  include/${PROJECT_NAME}/ui_node/ui_node.hpp
)
target_include_directories(ui_node PRIVATE include)
ament_target_dependencies(ui_node rclcpp std_msgs sensor_msgs OpenCV cv_bridge)

# 3. main_window (Qt GUI 애플리케이션) 실행 파일
add_executable(main_window
  src/main.cpp
  src/main_window.cpp
  include/${PROJECT_NAME}/main_window.hpp
  ui/mainwindow.ui
)
target_include_directories(main_window PRIVATE include)
ament_target_dependencies(main_window rclcpp std_msgs sensor_msgs Boost OpenCV cv_bridge)
target_link_libraries(main_window
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Network
  Qt5::Multimedia
  Qt5::MultimediaWidgets
)

# ui 파일 경로 지정
set_target_properties(main_window PROPERTIES
  AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

# Install targets
install(TARGETS
  camera_node
  ui_node
  main_window
  DESTINATION lib/${PROJECT_NAME}
)

# Install launch and config directories
install(DIRECTORY
  launch
  config
  DESTINATION share/${PROJECT_NAME}/
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()