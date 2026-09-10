#include "day3_hw1_package/parameter.hpp"

Parameter::Parameter(rclcpp::Node *node) : node_(node)
{
    node_->declare_parameter<int>("r", 0);
    node_->declare_parameter<int>("g", 0);
    node_->declare_parameter<int>("b", 0);
    node_->declare_parameter<int>("width", 1);

    r_ = node_->get_parameter("r").as_int();
    g_ = node_->get_parameter("g").as_int();
    b_ = node_->get_parameter("b").as_int();
    width_ = node_->get_parameter("width").as_int();
}
