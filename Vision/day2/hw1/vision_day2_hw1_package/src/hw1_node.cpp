#include "../include/vision_day2_hw1_package/hw1_node.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

Hw1Node::Hw1Node() : Node("hw1_node")
{
    image_sub = this->create_subscription<img>("/camera1/camera/compressed_image", 10,
                                               std::bind(&Hw1Node::imageCallback, this, std::placeholders::_1));

    info_sub = this->create_subscription<camInfo>("/camera1/compressed_info", 10,
                                                  std::bind(&Hw1Node::cameraInfoCallback, this, std::placeholders::_1));

    tilt_sub = this->create_subscription<tiltStatus>(
        "/camera1/pan_tilt_status", 10, std::bind(&Hw1Node::tiltStatusCallback, this, std::placeholders::_1));
}

Hw1Node::~Hw1Node()
{
    cv::destroyAllWindows();
}

// Z = f_x * W / w (W = 실제 크기, w = 화면 크기)
double Hw1Node::DistanceComputationA()
{
    if (fx == 0.0 || width <= 0)
        return 0.0;

    return correctionA * (fx * real_width) / (double)width;
}

double Hw1Node::DistanceComputationB()
{
    if (fy == 0.0)
        return 0.0;

    // yn = (v - cy) / fy, alpha = arctan(yn)
    double yn = (v - cy) / fy;
    double alpha = std::atan(yn);

    if (std::abs(std::tan(theta + alpha)) < 1e-6)
        return 0.0; // 분모 != 0이므로 예외처리

    // D = h / tan(theta + alpha)
    return correctionB * h / std::tan(theta + alpha);
}

void Hw1Node::imageCallback(const img::SharedPtr msg)
{
    cv_bridge::CvImagePtr cv_ptr;
    cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");

    cv::Mat image = cv_ptr->image;

    if (image.empty())
        std::cout << "경로 확인";

    // 블러링
    cv::medianBlur(image, image, 5);

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    // H 0 ~ 179, S 0 ~ 255, V 0 ~ 255
    // H 범위로 목표 색상을 선택, S 하한으로 저채도 영역을 제외, V 하한으로 저명도 영역을 제외

    cv::Mat mask_yellow;
    cv::inRange(hsv, cv::Scalar(20, 100, 100), cv::Scalar(35, 255, 255), mask_yellow);

    // Clsoing
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(mask_yellow, mask_yellow, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask_yellow, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > 30) // 작은 점 인식 방지
        {
            cv::Rect box = cv::boundingRect(contour);

            width = box.width;
            height = box.height;

            v = (double)(box.y + box.height); // box.y: 좌측 상단 꼭짓점 y

            double distanceA = DistanceComputationA();
            double distanceB = DistanceComputationB();

            cv::rectangle(image, box, cv::Scalar(0, 255, 255), 5);

            std::string text = "DistA: " + std::to_string(distanceA).substr(0, 4) + "m" +
                               " | DistB: " + std::to_string(distanceB).substr(0, 4) + "m";

            cv::putText(image, text, cv::Point(box.x, std::max(20, box.y - 10)), cv::FONT_HERSHEY_SIMPLEX, 0.6,
                        cv::Scalar(0, 255, 255), 2);

            break;
        }
    }

    cv::imshow("image", image);
    cv::waitKey(1); // 동영상
}

void Hw1Node::cameraInfoCallback(const camInfo::SharedPtr msg)
{
    fx = msg->k[0];
    fy = msg->k[4];
    cy = msg->k[5];
}

void Hw1Node::tiltStatusCallback(const tiltStatus::SharedPtr msg)
{
    double tilt_degree = msg->tilt / -1000.0;

    theta = tilt_degree * M_PI / 180.0; // rad
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Hw1Node>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}