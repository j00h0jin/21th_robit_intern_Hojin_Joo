#include "day1_hw2_ros2_cpp_package/day1_hw2_sub_node.hpp"

Day1Hw2SubNode::Day1Hw2SubNode() : Node("day1_hw2_sub_node")
{
    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        "string", 10, std::bind(&Day1Hw2SubNode::topic_callback, this, std::placeholders::_1));

    subInt_ = this->create_subscription<std_msgs::msg::Int32>(
        "int", 10, std::bind(&Day1Hw2SubNode::int_callback, this, std::placeholders::_1));

    subBool_ = this->create_subscription<std_msgs::msg::Bool>(
        "bool", 10, std::bind(&Day1Hw2SubNode::bool_callback, this, std::placeholders::_1));

    subFloat_ = this->create_subscription<std_msgs::msg::Float32>(
        "float", 10, std::bind(&Day1Hw2SubNode::float_callback, this, std::placeholders::_1));
}

void Day1Hw2SubNode::topic_callback(const std_msgs::msg::String::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}

void Day1Hw2SubNode::int_callback(const std_msgs::msg::Int32::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Int: '%d'", msg->data);
}

void Day1Hw2SubNode::bool_callback(const std_msgs::msg::Bool::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Bool: '%d'", msg->data);
}

void Day1Hw2SubNode::float_callback(const std_msgs::msg::Float32::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard Float: '%f'", msg->data);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day1Hw2SubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}