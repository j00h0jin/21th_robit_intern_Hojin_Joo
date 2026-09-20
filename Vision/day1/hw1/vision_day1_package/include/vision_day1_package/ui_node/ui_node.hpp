#pragma once

#include "cv_bridge/cv_bridge.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include <QImage>
#include <QObject>
#include <mutex>
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

    std::mutex img_mutex;
    QImage mutex_img;

    std::string topic_name;

  signals:
    void imageSignal(const QImage &img);
};
