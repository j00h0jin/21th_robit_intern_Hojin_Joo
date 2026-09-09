#include "day2_hw1_package/day2_hw1_sub_node.hpp"

#include <sstream>

Day2Hw1SubNode::Day2Hw1SubNode() : Node("day2_hw1_sub_node")
{
    subTwoInts_ = this->create_subscription<custom_interfaces::msg::AddTwoInts>(
        "topic_twoInts", 10, std::bind(&Day2Hw1SubNode::topic_callback, this, std::placeholders::_1));
}

void Day2Hw1SubNode::topic_callback(const custom_interfaces::msg::AddTwoInts::SharedPtr msg) const
{
    std::stringstream ss;
    ss << "sub: {a: " << msg->a << ", b: [";
    for (size_t i = 0; i < msg->b.size(); ++i)
    {
        ss << msg->b[i];
        if (i < msg->b.size() - 1)
        {
            ss << ", ";
        }
    }
    ss << "]}";

    RCLCPP_INFO(this->get_logger(), "%s",
                ss.str().c_str()); // .str(): string으로 변환, c_str() char* 형으로 변환
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Day2Hw1SubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}