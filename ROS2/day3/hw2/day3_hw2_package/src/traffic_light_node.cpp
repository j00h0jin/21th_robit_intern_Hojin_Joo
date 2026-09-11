/**
 * @file /src/traffic_light_node.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/traffic_light_node.hpp"

#include <chrono>

TrafficLightNode::TrafficLightNode() : Node("traffic_light_node")
{
    state_publisher_ = create_publisher<std_msgs::msg::String>("topic_traffic", 10);

    declare_parameter<double>("red_duration", 5.0);
    declare_parameter<double>("green_duration", 5.0);
    declare_parameter<double>("yellow_duration", 2.0);

    red_duration_ = get_parameter("red_duration").as_double();
    green_duration_ = get_parameter("green_duration").as_double();
    yellow_duration_ = get_parameter("yellow_duration").as_double();

    current_state_ = "GREEN";
    timer_ = create_wall_timer(std::chrono::milliseconds(16), std::bind(&TrafficLightNode::timer_callback, this));
}

void TrafficLightNode::timer_callback()
{
    const double timer_period = 0.016; // create_wall_timer(milliseconds(16), ...
    elapsed_time_ += timer_period;

    double state_duration = 0.0;

    if (current_state_ == "RED")
        state_duration = red_duration_;
    else if (current_state_ == "GREEN")
        state_duration = green_duration_;
    else
        state_duration = yellow_duration_;

    if (elapsed_time_ >= state_duration) // 해당 light duration이 지나면 다음 색으로 변경
    {
        // red -> green -> yellow -> red
        elapsed_time_ = 0.0;
        if (current_state_ == "RED")
            current_state_ = "GREEN";
        else if (current_state_ == "GREEN")
            current_state_ = "YELLOW";
        else
            current_state_ = "RED";
    }

    std_msgs::msg::String message;
    message.data = current_state_;
    state_publisher_->publish(message);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrafficLightNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
