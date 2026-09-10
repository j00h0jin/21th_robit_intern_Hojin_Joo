#pragma once

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"

class Day3Hw2SubNode : public rclcpp::Node
{
  public:
    Day3Hw2SubNode();

    std::string traffic_state()
    {
        return traffic_state_;
    }

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);

    std::string traffic_state_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_traffic;
};