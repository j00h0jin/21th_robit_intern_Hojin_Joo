#pragma once

#include "cv_bridge/cv_bridge.hpp"
#include "rclcpp/rclcpp.hpp"
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

using img = sensor_msgs::msg::Image;
using camInfo = sensor_msgs::msg::CameraInfo;

class Hw1Node : public rclcpp::Node
{
  public:
    Hw1Node();
    ~Hw1Node();

    int width;
    // int height;
    double fx = 0.0;           // /camera1/compressed_info의 k[0]
    double real_width = 0.055; // 1m = 1, 5.5cm = 0.055
    double correction = 0.50 / 0.29;

  private:
    rclcpp::Subscription<img>::SharedPtr image_sub;
    rclcpp::Subscription<camInfo>::SharedPtr info_sub;
    void imageCallback(const img::SharedPtr msg);
    void cameraInfoCallback(const camInfo::SharedPtr msg);
    double DistanceComputationA(int width);
    void DistanceComputationB();
};