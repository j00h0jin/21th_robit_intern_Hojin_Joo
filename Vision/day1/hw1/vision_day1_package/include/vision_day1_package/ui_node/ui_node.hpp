#pragma once

#include "cv_bridge/cv_bridge.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <QImage>
#include <QObject>
#include <opencv2/opencv.hpp>

using img = sensor_msgs::msg::Image;

class SubNode : public QObject, public rclcpp::Node
{
    Q_OBJECT

  public:
    SubNode();
    ~SubNode();

  private:
    rclcpp::Subscription<img>::SharedPtr image_sub;
    void imageCallback(const img::SharedPtr msg);

  signals:
    void imageSignal(const QImage &img);
};
