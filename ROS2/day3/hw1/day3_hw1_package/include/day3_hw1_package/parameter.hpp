#pragma once

#include "rclcpp/rclcpp.hpp"

class Parameter
{
  public:
    explicit Parameter(rclcpp::Node *node);

    int r() const
    {
        return r_;
    }
    int g() const
    {
        return g_;
    }
    int b() const
    {
        return b_;
    }
    int width() const
    {
        return width_;
    }

  private:
    rclcpp::Node *node_;
    int r_, g_, b_, width_;
};
