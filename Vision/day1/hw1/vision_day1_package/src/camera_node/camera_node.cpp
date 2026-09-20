#include "../include/vision_day1_package/camera_node/camera_node.hpp"
#include "cv_bridge/cv_bridge.hpp"
#include <iostream>

CameraNode::CameraNode() : Node("camera_node")
{
    capture.open(0);

    image_pub = this->create_publisher<sensor_msgs::msg::Image>("Image", 10);

    timer = this->create_wall_timer(std::chrono::milliseconds(16), std::bind(&CameraNode::timerCallback, this));
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
    header.frame_id = "camera_frame";

    auto msg = cv_bridge::CvImage(header, "bgr8", frame).toImageMsg();

    image_pub->publish(*msg);
}

int main(int argc, char *argv[])
{
    // cv::VideoCapture cap(0);
    // cv::Mat frame;
    // while (true)
    // {
    //     cap >> frame;
    //     if (frame.empty())
    //         break;
    //     cv::imshow("camera", frame);
    //     if (cv::waitKey(1) == 27)
    //         break;
    // }
    // cap.release();
    // return 0;

    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<CameraNode>());
    rclcpp::shutdown();
    return 0;
}