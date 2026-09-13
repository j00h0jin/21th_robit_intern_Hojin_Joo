cmake_minimum_required(VERSION 3.5)

project(day4_hw2_package)

# Default to C++17
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(lifecycle_msgs REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

### demos
add_executable(publisher_node
  src/publisher_node.cpp)
ament_target_dependencies(publisher_node
  "lifecycle_msgs"
  "rclcpp"
  "rclcpp_lifecycle"
  "std_msgs"
)
target_include_directories(publisher_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

add_executable(subscriber_node
  src/subscriber_node.cpp)
ament_target_dependencies(subscriber_node
  "lifecycle_msgs"
  "rclcpp"
  "std_msgs"
)

install(TARGETS
  publisher_node
  subscriber_node
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()