#include "day2_hw1_package/day2_hw1_pub_node.hpp"

Day2Hw1PubNode::Day2Hw1PubNode() : Node("day2_hw1_pub_node")
{
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic_string", 10);
    pubInt_ = this->create_publisher<std_msgs::msg::Int32>("topic_int", 10);
    pubBool_ = this->create_publisher<std_msgs::msg::Bool>("topic_bool", 10);
    pubFloat_ = this->create_publisher<std_msgs::msg::Float32>("topic_float", 10);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(2000), std::bind(&Day2Hw1PubNode::timer_callback, this));
}

void Day2Hw1PubNode::timer_callback()
{
    auto msgString = std_msgs::msg::String();
    msgString.data = "Hello ROS 2! " + std::to_string(count_++);
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", msgString.data.c_str());
    publisher_->publish(msgString);

    auto msgInt = std_msgs::msg::Int32();
    msgInt.data = 2024405020;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", msgInt.data);
    pubInt_->publish(msgInt);

    auto msgBool = std_msgs::msg::Bool();
    msgBool.data = true;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", msgBool.data);
    pubBool_->publish(msgBool);

    auto msgFloat = std_msgs::msg::Float32();
    msgFloat.data = 3.141592;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%f'", msgFloat.data);
    pubFloat_->publish(msgFloat);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day2Hw1PubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}