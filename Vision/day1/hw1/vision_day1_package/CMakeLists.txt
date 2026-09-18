cmake_minimum_required(VERSION 3.8)
project(vision_day1_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_package(Qt5 COMPONENTS Widgets Core Gui Network MultimediaWidgets REQUIRED)
find_package(OpenCV REQUIRED)

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

# 1. camera_node Executable
add_executable(camera_node
  src/camera_node/camera_node.cpp
  include/${PROJECT_NAME}/camera_node/camera_node.hpp
)
target_include_directories(camera_node PRIVATE include)
ament_target_dependencies(camera_node rclcpp std_msgs)
target_link_libraries(camera_node ${OpenCV_LIBRARIES})

# 2. ui_node Executable (Qt UI Node)
add_executable(ui_node
  src/ui_node/ui_node.cpp
  include/${PROJECT_NAME}/ui_node/ui_node.hpp
  ui/mainwindow.ui
)
target_include_directories(ui_node PRIVATE include)
ament_target_dependencies(ui_node rclcpp std_msgs Boost)
target_link_libraries(ui_node
  Qt5::Widgets
  Qt5::Core
  Qt5::Gui
  Qt5::Network
  Qt5::MultimediaWidgets
  ${OpenCV_LIBRARIES}
)

# ui 파일 경로 지정
set_target_properties(ui_node PROPERTIES
  AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

# Install targets
install(TARGETS
  camera_node
  ui_node
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