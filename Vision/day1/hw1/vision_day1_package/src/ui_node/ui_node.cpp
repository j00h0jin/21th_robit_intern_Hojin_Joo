#include "../include/vision_day1_package/ui_node/ui_node.hpp"

#include <iostream>

SubNode::SubNode() : QObject(nullptr), Node("ui_node")
{
    topic_name = this->declare_parameter<std::string>("topic_name", "Image");

    image_sub =
        this->create_subscription<img>(topic_name, 10, std::bind(&SubNode::imageCallback, this, std::placeholders::_1));
}

SubNode::~SubNode()
{
    cv::destroyAllWindows();
}

void SubNode::imageCallback(const img::SharedPtr msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");

    cv::Mat image = cv_ptr->image;
    if (image.empty())
        return;

    // BGR -> RGB
    cv::Mat rgb_image;
    cv::cvtColor(image, rgb_image, cv::COLOR_BGR2RGB);

    QImage qimg(rgb_image.data, rgb_image.cols, rgb_image.rows, static_cast<int>(rgb_image.step),
                QImage::Format_RGB888);

    {
        // 만약 다른 곳에서 해당 이미지에 대한 접근 시도가 있을 때 mutex탈출까지 대기시킴
        std::lock_guard<std::mutex> lock(img_mutex);
        mutex_img = qimg.copy();
    }

    emit imageSignal(mutex_img);
}