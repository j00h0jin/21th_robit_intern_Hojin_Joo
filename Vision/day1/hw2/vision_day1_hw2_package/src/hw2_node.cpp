#include "../include/vision_day1_hw2_package/hw2_node.hpp"
#include <iostream>

Hw2Node::Hw2Node() : Node("hw2_node")
{
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Hw2Node>();

    cv::VideoCapture cap(0);
    cv::Mat frame;
    while (rclcpp::ok())
    {
        cap >> frame;
        if (frame.empty())
            break;
        cv::imshow("camera", frame);
        if (cv::waitKey(1) == 27)
            break;

        rclcpp::spin_some(node);
    }
    cap.release();
    cv::destroyAllWindows();
    rclcpp::shutdown();
    return 0;
}