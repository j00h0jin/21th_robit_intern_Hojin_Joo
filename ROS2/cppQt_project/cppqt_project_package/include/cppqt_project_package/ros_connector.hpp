#pragma once

#include "geometry_msgs/msg/point.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_srvs/srv/empty.hpp"
#include <QObject>

class RosConnector : public QObject, public rclcpp::Node
{
    Q_OBJECT
  public:
    RosConnector();

    void publishCommand(double dx, double dy);
    void requestReset();
    void requestGameOver();

  signals:
    void playerUpdated(int x, int y, bool exhausted);
    void staminaUpdated(float stamina);
    void chaserUpdated(int x, int y);

  private:
    void playerCallback(const geometry_msgs::msg::Point::SharedPtr msg);
    void staminaCallback(const std_msgs::msg::Float32::SharedPtr msg);
    void chaserCallback(const geometry_msgs::msg::Point::SharedPtr msg);

    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr command_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr player_sub_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr stamina_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr chaser_sub_;

    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr reset_player_client_;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr reset_chaser_client_;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr game_over_client_;
};