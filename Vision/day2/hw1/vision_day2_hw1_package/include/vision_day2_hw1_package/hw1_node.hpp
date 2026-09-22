#pragma once

#include "cv_bridge/cv_bridge.hpp"
#include "rclcpp/rclcpp.hpp"
#include <insta360_usb_cam/msg/pan_tilt_status_msgs.hpp>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/camera_info.hpp>
#include <sensor_msgs/msg/image.hpp>

using img = sensor_msgs::msg::Image;
using camInfo = sensor_msgs::msg::CameraInfo;
using tiltStatus = insta360_usb_cam::msg::PanTiltStatusMsgs;

class Hw1Node : public rclcpp::Node
{
  public:
    Hw1Node();
    ~Hw1Node();

    int width;
    int height;
    double fx = 0.0;           // /camera1/compressed_info의 k[0]
    double real_width = 0.055; // 1m = 1, 5.5cm = 0.055
    double correctionA = 0.50 / 0.29;

    double fy = 0.0;         // /camera1/compressed_info의 k[4]
    double cy = 0.0;         // /camera1/compressed_info의 k[5]
    double v = 0.0;          // 물체의 y축 바닥 좌표
    double theta = 0.0;      // tilt 각도
    double h = 0.175 + 0.05; // 가로 10cm, 세로 17.5cm, 카메라 5cm
    double correctionB = 1.00 / 0.12;

  private:
    rclcpp::Subscription<img>::SharedPtr image_sub;
    rclcpp::Subscription<camInfo>::SharedPtr info_sub;
    rclcpp::Subscription<tiltStatus>::SharedPtr tilt_sub;
    void imageCallback(const img::SharedPtr msg);
    void cameraInfoCallback(const camInfo::SharedPtr msg);
    void tiltStatusCallback(const tiltStatus::SharedPtr msg);
    double DistanceComputationA();
    double DistanceComputationB();
};