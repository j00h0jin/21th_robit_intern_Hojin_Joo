#include "day1_hw2_ros2_cpp_package/day1_hw2_pub_node.hpp"

Day1Hw2PubNode::Day1Hw2PubNode() : Node("day1_hw2_pub_node")
{
    publisher_ = this->create_publisher<std_msgs::msg::String>("day1_hw2_topic", 10);
    timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&Day1Hw2PubNode::timer_callback, this));
}

void Day1Hw2PubNode::timer_callback()
{
    auto message = std_msgs::msg::String();
    message.data = "Hello ROS 2! " + std::to_string(++count_); // 필요시 count 변수 추가 또는 단순 문자열 사용
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day1Hw2PubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}