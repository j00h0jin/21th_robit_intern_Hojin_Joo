#ifndef DAY1_HW2_PUB_NODE_HPP_
#define DAY1_HW2_PUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class Day1Hw2PubNode : public rclcpp::Node
{
  public:
    Day1Hw2PubNode();

  private:
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback();

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

    size_t count_ = 0;
};

#endif