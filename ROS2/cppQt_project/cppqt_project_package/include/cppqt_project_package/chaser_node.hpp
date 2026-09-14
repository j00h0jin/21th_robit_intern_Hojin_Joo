#pragma once

#include "cppqt_project_package/grid_map.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/empty.hpp"

class ChaserNode : public rclcpp::Node
{
  public:
    ChaserNode();

  private:
    void playerPosCallback(const geometry_msgs::msg::Point::SharedPtr msg);
    void resetCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                       std::shared_ptr<std_srvs::srv::Empty::Response> res);

    void updateChaser();
    void singleStep();

    bool isPathClear(int x1, int y1, int x2, int y2);

    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr chaser_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr player_pos_sub_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr reset_srv_; // 리셋 서비스 서버
    rclcpp::TimerBase::SharedPtr timer_;

    GridMap grid_map_;
    int grid_size_;

    int chaser_start_x_;
    int chaser_start_y_;
    int chaser_x_;
    int chaser_y_;
    double player_x_;
    double player_y_;

    double momentum_factor_;
    int last_dx_;
    int last_dy_;
    bool is_turning_delay_;
};