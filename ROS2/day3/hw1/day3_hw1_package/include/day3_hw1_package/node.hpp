#ifndef DAY3_HW1_PACKAGE_NODE_HPP_
#define DAY3_HW1_PACKAGE_NODE_HPP_

#include <atomic>
#include <cstdint>
#include <memory>
#include <thread>

#include "day3_hw1_package/parameter.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "rclcpp/rclcpp.hpp"
#include "turtlesim/srv/set_pen.hpp"

class TurtlesimControl : public rclcpp::Node
{
  public:
    TurtlesimControl();
    ~TurtlesimControl() override;

  private:
    void keyboardListenerLoop();
    void controlLoopCallback();
    void setPen(std::uint8_t off = 0);
    void circle();
    void triangle();
    void rectangle();
    void pentagon();

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr pen_client_;
    rclcpp::TimerBase::SharedPtr control_timer_;
    std::thread keyboard_thread_;
    std::atomic<int> step_{0};
    std::atomic<char> current_mode_{'\0'};
    std::atomic<bool> is_running_{true};

    std::unique_ptr<Parameter> config_;
};

#endif
