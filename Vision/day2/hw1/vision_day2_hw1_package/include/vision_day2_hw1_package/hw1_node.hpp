#pragma once

#include "cv_bridge/cv_bridge.hpp"
#include "rclcpp/rclcpp.hpp"
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <sensor_msgs/msg/image.hpp>

using img = sensor_msgs::msg::Image;

class Hw1Node : public rclcpp::Node
{
  public:
    Hw1Node();
    ~Hw1Node();

  private:
    rclcpp::Subscription<img>::SharedPtr image_sub;
    void imageCallback(const img::SharedPtr msg);
};