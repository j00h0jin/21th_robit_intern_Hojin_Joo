#ifndef DAY2_HW1_PUB_NODE_HPP_
#define DAY2_HW1_PUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

#include "custom_interfaces/msg/add_two_ints.hpp"

class Day2Hw1PubNode : public rclcpp::Node
{
  public:
    Day2Hw1PubNode();

  private:
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback();

    rclcpp::Publisher<custom_interfaces::msg::AddTwoInts>::SharedPtr pubTwoInts_;

    size_t count_ = 0;
};

#endif