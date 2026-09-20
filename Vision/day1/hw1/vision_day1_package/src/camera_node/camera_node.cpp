#include "../include/vision_day1_package/camera_node/camera_node.hpp"
#include "cv_bridge/cv_bridge.hpp"
#include <iostream>

CameraNode::CameraNode() : Node("camera_node")
{
    capture.open(0);

    Hz = this->declare_parameter<int>("Hz", 16);
    frame_id = this->declare_parameter<std::string>("frame_id", "camera_frame");
    encoding = this->declare_parameter<std::string>("encoding", "bgr8");
    topic_name = this->declare_parameter<std::string>("topic_name", "Image");

    image_pub = this->create_publisher<sensor_msgs::msg::Image>(topic_name, 10);

    timer = this->create_wall_timer(std::chrono::milliseconds(Hz), std::bind(&CameraNode::timerCallback, this));
}

CameraNode::~CameraNode()
{
    if (capture.isOpened())
        capture.release();
}

void CameraNode::timerCallback()
{
    cv::Mat frame;
    capture >> frame;

    std_msgs::msg::Header header;
    header.stamp = this->now();
    header.frame_id = frame_id;

    auto msg = cv_bridge::CvImage(header, encoding, frame).toImageMsg();

    image_pub->publish(*msg);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraNode>());
    rclcpp::shutdown();
    return 0;
}