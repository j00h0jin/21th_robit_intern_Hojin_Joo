#ifndef DAY1_HW2_SUB_NODE_HPP_
#define DAY1_HW1_SUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Day1Hw2SubNode : public rclcpp::Node
{
  public:
    Day1Hw2SubNode();

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};

#endif