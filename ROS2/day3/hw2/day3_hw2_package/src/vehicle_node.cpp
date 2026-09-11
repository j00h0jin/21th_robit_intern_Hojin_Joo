/**
 * @file /src/vehicle_node.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/vehicle_node.hpp"

VehicleNode::VehicleNode() : Node("vehicle_node")
{
    position_publisher_ = create_publisher<geometry_msgs::msg::Pose2D>("topic_vehicle", 10);

    declare_parameter<double>("start_x", 0.0);
    declare_parameter<double>("start_y", 0.0);
    declare_parameter<double>("speed", 1.0);
    declare_parameter<std::string>("acceleration", "x");
    declare_parameter<double>("stop_line_position", 0.0);
    declare_parameter<double>("road_length", 20.0);
    declare_parameter<double>("speed", 1.0);
    declare_parameter<double>("acceleration", 0.5);
    declare_parameter<double>("deceleration", 0.5);
    declare_parameter<double>("max_speed", 2.0);

    start_x_ = get_parameter("start_x").as_double();
    start_y_ = get_parameter("start_y").as_double();
    direction_ = get_parameter("direction").as_string();
    stop_line_position_ = get_parameter("stop_line_position").as_double();
    road_length_ = get_parameter("road_length").as_double();
    speed_ = get_parameter("speed").as_double();
    acceleration_ = get_parameter("acceleration").as_double();
    deceleration_ = get_parameter("deceleration").as_double();
    max_speed_ = get_parameter("max_speed").as_double();

    x_ = get_parameter("start_x").as_double();
    y_ = get_parameter("start_y").as_double();
    vx_ = get_parameter("speed").as_double();
    vy_ = get_parameter("speed").as_double();

    timer_ = create_wall_timer(std::chrono::milliseconds(16), std::bind(&VehicleNode::timer_callback, this));
}

void VehicleNode::timer_callback()
{
    // 속도 계산 후 x, y
    // v = v_0 + at
    // s = v_0t + 1/2(at^2)

    geometry_msgs::msg::Pose2D message;
    message.x = x_;
    message.y = y_;
    message.theta = 0.0;
    position_publisher_->publish(message);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VehicleNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
