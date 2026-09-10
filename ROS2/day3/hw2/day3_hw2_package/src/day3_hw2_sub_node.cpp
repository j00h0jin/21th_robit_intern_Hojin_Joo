#include "day3_hw2_package/day3_hw2_sub_node.hpp"

Day3Hw2SubNode::Day3Hw2SubNode() : Node("day3_hw2_sub_node")
{
    sub_traffic = this->create_subscription<std_msgs::msg::String>(
        "topic_traffic", 10, std::bind(&Day3Hw2SubNode::topic_callback, this, std::placeholders::_1));
}

void Day3Hw2SubNode::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    traffic_state_ = msg->data;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day3Hw2SubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}