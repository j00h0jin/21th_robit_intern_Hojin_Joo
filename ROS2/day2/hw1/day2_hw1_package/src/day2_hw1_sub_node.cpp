#include "day2_hw1_package/day2_hw1_sub_node.hpp"

Day2Hw1SubNode::Day2Hw1SubNode() : Node("day2_hw1_sub_node")
{
    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        "topic_string", 10, std::bind(&Day2Hw1SubNode::topic_callback, this, std::placeholders::_1));

    subInt_ = this->create_subscription<std_msgs::msg::Int32>(
        "topic_int", 10, std::bind(&Day2Hw1SubNode::int_callback, this, std::placeholders::_1));

    subBool_ = this->create_subscription<std_msgs::msg::Bool>(
        "topic_bool", 10, std::bind(&Day2Hw1SubNode::bool_callback, this, std::placeholders::_1));

    subFloat_ = this->create_subscription<std_msgs::msg::Float32>(
        "topic_float", 10, std::bind(&Day2Hw1SubNode::float_callback, this, std::placeholders::_1));
}

void Day2Hw1SubNode::topic_callback(const std_msgs::msg::String::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}

void Day2Hw1SubNode::int_callback(const std_msgs::msg::Int32::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Int: '%d'", msg->data);
}

void Day2Hw1SubNode::bool_callback(const std_msgs::msg::Bool::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Bool: '%d'", msg->data);
}

void Day2Hw1SubNode::float_callback(const std_msgs::msg::Float32::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Float: '%f'", msg->data);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day2Hw1SubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}