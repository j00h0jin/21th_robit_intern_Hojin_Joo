#include "../include/vision_day1_package/camera_node/camera_node.hpp"
#include <iostream>

CameraNode::CameraNode() : Node("camera_node")
{
}

int main()
{
    cv::VideoCapture cap(0);
    cv::Mat frame;
    while (true)
    {
        cap >> frame;
        if (frame.empty())
            break;
        cv::imshow("camera", frame);
        if (cv::waitKey(1) == 27)
            break;
    }
    cap.release();
    return 0;
}