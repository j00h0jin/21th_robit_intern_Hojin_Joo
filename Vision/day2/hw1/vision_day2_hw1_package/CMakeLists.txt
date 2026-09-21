cmake_minimum_required(VERSION 3.8)
project(vision_day2_hw1_package)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(ament_index_cpp REQUIRED)

include_directories(include)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(hw1_node src/hw1_node.cpp)
ament_target_dependencies(hw1_node 
  rclcpp 
  std_msgs 
  sensor_msgs 
  cv_bridge
  ament_index_cpp
)

target_link_libraries(hw1_node ${OpenCV_LIBRARIES})

install(TARGETS
  hw1_node
  DESTINATION lib/${PROJECT_NAME}
)

# install(DIRECTORY resources
#   DESTINATION share/${PROJECT_NAME}
# )

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)

ament_package()