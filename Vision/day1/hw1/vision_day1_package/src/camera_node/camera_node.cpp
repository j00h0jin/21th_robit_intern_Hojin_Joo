#include "../include/vision_day1_package/camera_node/camera_node.hpp"

CameraNode::CameraNode() : Node("camera_node")
{
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CameraNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}