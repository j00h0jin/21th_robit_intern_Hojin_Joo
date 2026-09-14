#pragma once

#include "cppqt_project_package/grid_map.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_srvs/srv/empty.hpp"

class PlayerNode : public rclcpp::Node
{
  public:
    PlayerNode();

  private:
    void commandCallback(const geometry_msgs::msg::Point::SharedPtr msg);
    void resetCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                       std::shared_ptr<std_srvs::srv::Empty::Response> res);
    void gameOverCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                          std::shared_ptr<std_srvs::srv::Empty::Response> res);
    void staminaTimerCallback();
    bool isTouchingWall() const;

    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr player_pub_;
    rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr stamina_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr command_sub_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr reset_srv_;
    rclcpp::Service<std_srvs::srv::Empty>::SharedPtr game_over_srv_;
    rclcpp::TimerBase::SharedPtr stamina_timer_;

    GridMap grid_map_;
    int grid_size_;

    int player_start_x_;
    int player_start_y_;
    int player_x_;
    int player_y_;

    double max_stamina_;
    double consumption_rate_;
    double recovery_rate_;
    double wall_stamina_;
    bool stamina_exhausted_;
    int exhaust_counter_;

    bool game_over_;
};