cmake_minimum_required(VERSION 3.5)
project(day3_hw2_package)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ament_cmake REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui Network REQUIRED)
find_package(std_msgs REQUIRED)

set(PACKAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/main_window.cpp
  ui/mainwindow.ui
  resources/images.qrc
  include/${PROJECT_NAME}/main_window.hpp
)
target_include_directories(${PROJECT_NAME} PRIVATE ${PACKAGE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Network)
set_target_properties(${PROJECT_NAME} PROPERTIES
  AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui"
)

add_executable(traffic_light_node
  src/traffic_light_node.cpp
  include/${PROJECT_NAME}/traffic_light_node.hpp
)
target_include_directories(traffic_light_node PRIVATE ${PACKAGE_INCLUDE_DIR})
ament_target_dependencies(traffic_light_node rclcpp std_msgs)

add_executable(vehicle_node
  src/vehicle_node.cpp
  include/${PROJECT_NAME}/vehicle_node.hpp
)
target_include_directories(vehicle_node PRIVATE ${PACKAGE_INCLUDE_DIR})
ament_target_dependencies(vehicle_node rclcpp std_msgs geometry_msgs)

add_executable(day3_hw2_sub_node
  src/day3_hw2_sub_node.cpp
  include/${PROJECT_NAME}/day3_hw2_sub_node.hpp
)
target_include_directories(day3_hw2_sub_node PRIVATE ${PACKAGE_INCLUDE_DIR})
ament_target_dependencies(day3_hw2_sub_node rclcpp std_msgs)

install(TARGETS
  ${PROJECT_NAME}
  traffic_light_node
  vehicle_node
  day3_hw2_sub_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch config DESTINATION share/${PROJECT_NAME}/)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()