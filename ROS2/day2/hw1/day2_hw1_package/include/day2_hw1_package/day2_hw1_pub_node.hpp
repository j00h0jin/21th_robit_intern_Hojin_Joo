#ifndef DAY1_HW2_PUB_NODE_HPP_
#define DAY1_HW2_PUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/string.hpp"

class Day2Hw1PubNode : public rclcpp::Node
{
  public:
    Day2Hw1PubNode();

  private:
    rclcpp::TimerBase::SharedPtr timer_;
    void timer_callback();

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr pubInt_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pubBool_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr pubFloat_;

    size_t count_ = 0;
};

#endif