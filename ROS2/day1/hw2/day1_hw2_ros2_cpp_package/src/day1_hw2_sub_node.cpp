#include "day1_hw2_ros2_cpp_package/day1_hw2_sub_node.hpp"

Day1Hw2SubNode::Day1Hw2SubNode() : Node("day1_hw2_sub_node")
{
    subscriber_ = this->create_subscription<std_msgs::msg::String>(
        "day1_hw2_topic", 10, std::bind(&Day1Hw2SubNode::topic_callback, this, std::placeholders::_1));
}

void Day1Hw2SubNode::topic_callback(const std_msgs::msg::String::SharedPtr msg) const
{
    RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day1Hw2SubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}