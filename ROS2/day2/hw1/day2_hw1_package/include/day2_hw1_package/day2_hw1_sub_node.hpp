#ifndef DAY1_HW2_SUB_NODE_HPP_
#define DAY1_HW1_SUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/string.hpp"

class Day2Hw1SubNode : public rclcpp::Node
{
  public:
    Day2Hw1SubNode();

  private:
    void topic_callback(const std_msgs::msg::String::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;

    void int_callback(const std_msgs::msg::Int32::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr subInt_;

    void bool_callback(const std_msgs::msg::Bool::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr subBool_;

    void float_callback(const std_msgs::msg::Float32::SharedPtr msg) const;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr subFloat_;
};

#endif