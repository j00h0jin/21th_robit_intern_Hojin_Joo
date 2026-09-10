/**
 * @file /src/traffic_light_node.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/traffic_light_node.hpp"

TrafficLightNode::TrafficLightNode() : Node("traffic_light_node")
{
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TrafficLightNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
