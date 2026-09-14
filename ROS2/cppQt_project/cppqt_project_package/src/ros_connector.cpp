#include "cppqt_project_package/ros_connector.hpp"

RosConnector::RosConnector()
    : Node("game_gui_node", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true))
{
    command_pub_ = this->create_publisher<geometry_msgs::msg::Point>("player_command", 10);

    player_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "player_position", 10, std::bind(&RosConnector::playerCallback, this, std::placeholders::_1));
    stamina_sub_ = this->create_subscription<std_msgs::msg::Float32>(
        "player_stamina", 10, std::bind(&RosConnector::staminaCallback, this, std::placeholders::_1));
    chaser_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "chaser_position", 10, std::bind(&RosConnector::chaserCallback, this, std::placeholders::_1));

    reset_player_client_ = this->create_client<std_srvs::srv::Empty>("reset_player");
    reset_chaser_client_ = this->create_client<std_srvs::srv::Empty>("reset_chaser");
    game_over_client_ = this->create_client<std_srvs::srv::Empty>("game_over_player");
}

void RosConnector::publishCommand(double dx, double dy)
{
    geometry_msgs::msg::Point msg;
    msg.x = dx;
    msg.y = dy;
    msg.z = 0.0;
    command_pub_->publish(msg);
}

void RosConnector::requestReset()
{
    auto req = std::make_shared<std_srvs::srv::Empty::Request>();
    reset_player_client_->async_send_request(req);
    reset_chaser_client_->async_send_request(req);
}

void RosConnector::requestGameOver()
{
    auto req = std::make_shared<std_srvs::srv::Empty::Request>();
    game_over_client_->async_send_request(req);
}

void RosConnector::playerCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    emit playerUpdated(static_cast<int>(msg->x), static_cast<int>(msg->y), msg->z == 1.0);
}

void RosConnector::staminaCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
    emit staminaUpdated(msg->data);
}

void RosConnector::chaserCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    emit chaserUpdated(static_cast<int>(msg->x), static_cast<int>(msg->y));
}