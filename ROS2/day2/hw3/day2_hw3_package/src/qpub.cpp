/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day2_hw3_package/qpub.hpp"

QPub::QPub()
{
    node = rclcpp::Node::make_shared("day2_hw3_pub");
    publisher_ = node->create_publisher<std_msgs::msg::String>("topic_string", 10);
    this->start();
}

QPub::~QPub()
{
}

void QPub::run()
{
    rclcpp::WallRate loop_rate(20);
    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    Q_EMIT rosShutDown();
}

void QPub::pubString(QString msg)
{

    auto msgString = std_msgs::msg::String();
    msgString.data = msg.toUtf8().toStdString();
    // msgString.data = "Hello ROS 2! ";
    RCLCPP_INFO(node->get_logger(), "Publishing: '%s'", msgString.data.c_str());
    publisher_->publish(msgString);
}
