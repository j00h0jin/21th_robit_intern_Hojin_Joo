#pragma once

/*****************************************************************************
** Includes
*****************************************************************************/
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

/*****************************************************************************
** Class
*****************************************************************************/
class TrafficLightNode : public rclcpp::Node
{
  public:
    TrafficLightNode();

  private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr state_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string current_state_;
    double red_duration_;
    double green_duration_;
    double yellow_duration_;
    double elapsed_time_{0.0};

    void timer_callback();
};
