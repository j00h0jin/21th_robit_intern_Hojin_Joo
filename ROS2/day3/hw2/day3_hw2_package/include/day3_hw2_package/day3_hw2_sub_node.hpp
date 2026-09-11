#pragma once

#include "rclcpp/rclcpp.hpp"

#include "std_msgs/msg/string.hpp"
#include <geometry_msgs/msg/pose2_d.hpp>

class Day3Hw2SubNode : public rclcpp::Node
{
  public:
    Day3Hw2SubNode();

    std::string traffic_state()
    {
        return traffic_state_;
    }

    geometry_msgs::msg::Pose2D vehicle_position() const
    {
        return cur_coordinate_;
    }

  private:
    void traffic_topic_callback(const std_msgs::msg::String::SharedPtr msg);

    std::string traffic_state_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr sub_traffic;

    void vehicle_topic_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg);

    geometry_msgs::msg::Pose2D cur_coordinate_;
    rclcpp::Subscription<geometry_msgs::msg::Pose2D>::SharedPtr sub_vehicle;
};