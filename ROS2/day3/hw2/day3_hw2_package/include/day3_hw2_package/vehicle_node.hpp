#pragma once

/*****************************************************************************
** Includes
*****************************************************************************/
#include <geometry_msgs/msg/pose2_d.hpp>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>

/*****************************************************************************
** Class
*****************************************************************************/
class VehicleNode : public rclcpp::Node
{
  public:
    VehicleNode();

  private:
    rclcpp::Publisher<geometry_msgs::msg::Pose2D>::SharedPtr position_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    double start_x_{0.0};
    double start_y_{10.0};
    std::string direction_;
    double stop_line_position_{0.0};
    double road_length_{20.0};
    double speed_{1.0};
    double acceleration_{0.5};
    double deceleration_{0.5};
    double emergency_deceleration_{2.0};
    double max_speed_{2.0};

    double x_{0.0}; // 현재 좌표
    double y_{0.0};

    double vx_{0.0}; // 현재 속도
    double vy_{0.0};

    double &current_velocity();
    double &current_position();

    void timer_callback();

    void traffic_callback(const std_msgs::msg::String::SharedPtr message);
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr traffic_subscription_;
    std::string current_light_{"GREEN"};
};
