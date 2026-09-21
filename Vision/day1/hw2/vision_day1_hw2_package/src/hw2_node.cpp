#include "../include/vision_day1_hw2_package/hw2_node.hpp"
#include <ament_index_cpp/get_package_share_directory.hpp>
#include <iostream>
#include <string>
#include <vector>

Hw2Node::Hw2Node() : Node("hw2_node")
{
}

int main()
{
    std::string package_path = ament_index_cpp::get_package_share_directory("vision_day1_hw2_package");
    std::string img_path = package_path + "/resources/images/hw2.png";
    cv::Mat image = cv::imread(img_path);

    if (image.empty())
        std::cout << "경로 확인";

    // 블러링
    cv::medianBlur(image, image, 5);

    cv::Mat hsv;
    cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);
    // H 0 ~ 179, S 0 ~ 255, V 0 ~ 255, H: 빨간색 끝에 걸쳐있음
    // H 범위로 목표 색상을 선택, S 하한으로 저채도 영역을 제외, V 하한으로 저명도 영역을 제외

    cv::Mat mask1_red, mask2_red, mask_red;
    cv::inRange(hsv, cv::Scalar(0, 100, 100), cv::Scalar(9, 255, 255), mask1_red);
    cv::inRange(hsv, cv::Scalar(170, 100, 100), cv::Scalar(179, 255, 255), mask2_red);
    cv::bitwise_or(mask1_red, mask2_red, mask_red);
    cv::Mat mask_green, mask_blue;
    cv::inRange(hsv, cv::Scalar(33, 60, 60), cv::Scalar(85, 255, 255), mask_green);
    cv::inRange(hsv, cv::Scalar(95, 90, 90), cv::Scalar(125, 255, 255), mask_blue);

    // Clsoing
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
    cv::morphologyEx(mask_red, mask_red, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask_green, mask_green, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask_blue, mask_blue, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask_red, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > 100) // 작은 점 인식 방지
        {
            cv::Rect box = cv::boundingRect(contour);
            cv::rectangle(image, box, cv::Scalar(0, 0, 255), 5);
            break;
        }
    }
    // cv::Rect box = cv::boundingRect(contours[0]);
    // cv::rectangle(image, box, cv::Scalar(255, 0, 0), 5);

    cv::findContours(mask_green, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > 100)
        {
            cv::Rect box2 = cv::boundingRect(contour);
            cv::rectangle(image, box2, cv::Scalar(0, 255, 0), 5);
            break;
        }
    }

    cv::findContours(mask_blue, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (const auto &contour : contours)
    {
        double area = cv::contourArea(contour);
        if (area > 100)
        {
            cv::Rect box3 = cv::boundingRect(contour);
            cv::rectangle(image, box3, cv::Scalar(255, 0, 0), 5);
            break;
        }
    }

    cv::imshow("image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}