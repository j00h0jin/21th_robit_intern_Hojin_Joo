#pragma once
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <opencv2/opencv.hpp>

class CameraNode : public rclcpp::Node
{
  public:
    CameraNode();
    ~CameraNode();

  private:
    void timerCallback();

    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub;
    rclcpp::TimerBase::SharedPtr timer;

    cv::VideoCapture capture;

    int Hz;
    std::string frame_id;
    std::string encoding;
    std::string topic_name;
};