#include "day3_hw2_package/day3_hw2_sub_node.hpp"

Day3Hw2SubNode::Day3Hw2SubNode() : Node("day3_hw2_sub_node")
{
    sub_traffic = this->create_subscription<std_msgs::msg::String>(
        "topic_traffic", 10, std::bind(&Day3Hw2SubNode::traffic_topic_callback, this, std::placeholders::_1));

    sub_vehicle = this->create_subscription<geometry_msgs::msg::Pose2D>(
        "topic_vehicle", 10, std::bind(&Day3Hw2SubNode::vehicle_topic_callback, this, std::placeholders::_1));
}

void Day3Hw2SubNode::traffic_topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    traffic_state_ = msg->data;
}

void Day3Hw2SubNode::vehicle_topic_callback(const geometry_msgs::msg::Pose2D::SharedPtr msg)
{
    cur_coordinate_ = *msg;
}