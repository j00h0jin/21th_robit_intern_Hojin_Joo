#include "day2_hw1_package/day2_hw1_pub_node.hpp"

#include <sstream>

Day2Hw1PubNode::Day2Hw1PubNode() : Node("day2_hw1_pub_node")
{
    pubTwoInts_ = this->create_publisher<custom_interfaces::msg::AddTwoInts>("topic_twoInts", 10);

    timer_ = this->create_wall_timer(std::chrono::milliseconds(2000), std::bind(&Day2Hw1PubNode::timer_callback, this));
}

void Day2Hw1PubNode::timer_callback()
{
    auto msgTwoInts = custom_interfaces::msg::AddTwoInts();
    msgTwoInts.a = 2147483648; // Int64
    msgTwoInts.b = {1, 2, 3};  // Int32[]
    // RCLCPP_INFO(this->get_logger(), "Publishing: {a: %lld b:", msgTwoInts.a);
    // for (int i = 0; i < msgTwoInts.b.size(); i++)
    // {
    //     RCLCPP_INFO(this->get_logger(), " ,%d", msgTwoInts.b[i]);
    // }
    // RCLCPP_INFO(this->get_logger(), "}");

    std::stringstream ss;
    ss << "Pub: {a: " << msgTwoInts.a << ", b: [";
    for (size_t i = 0; i < msgTwoInts.b.size(); ++i)
    {
        ss << msgTwoInts.b[i];
        if (i < msgTwoInts.b.size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]}";

    RCLCPP_INFO(this->get_logger(), "%s", ss.str().c_str()); // ss.str(): stream을 string으로 변환
    pubTwoInts_->publish(msgTwoInts);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day2Hw1PubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}