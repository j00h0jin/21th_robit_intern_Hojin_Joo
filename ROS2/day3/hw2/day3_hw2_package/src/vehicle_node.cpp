/**
 * @file /src/vehicle_node.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/vehicle_node.hpp"

VehicleNode::VehicleNode() : Node("vehicle_node")
{
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VehicleNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
