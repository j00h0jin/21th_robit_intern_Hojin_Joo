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

#include "../include/day2_hw3_package/qsub.hpp"

QSub::QSub()
{
    node = rclcpp::Node::make_shared("day2_hw3_sub");
    subscriber_ = node->create_subscription<std_msgs::msg::String>(
        "topic_string", 10, std::bind(&QSub::topic_callback, this, std::placeholders::_1));
    this->start();
}

QSub::~QSub()
{
}

void QSub::run()
{
    rclcpp::WallRate loop_rate(20);
    while (rclcpp::ok())
    {
        rclcpp::spin_some(node);
        loop_rate.sleep();
    }
    Q_EMIT rosShutDown();
}

void QSub::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(node->get_logger(), "I heard: '%s'", msg->data.c_str());
    Q_EMIT receivedString(QString::fromStdString(msg->data));
}
