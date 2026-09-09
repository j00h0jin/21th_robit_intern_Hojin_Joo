#ifndef DAY2_HW1_SUB_NODE_HPP_
#define DAY2_HW1_SUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/add_two_ints.hpp"

class Day2Hw1SubNode : public rclcpp::Node
{
  public:
    Day2Hw1SubNode();

  private:
    void topic_callback(const custom_interfaces::msg::AddTwoInts::SharedPtr msg) const;
    rclcpp::Subscription<custom_interfaces::msg::AddTwoInts>::SharedPtr subTwoInts_;
};

#endif