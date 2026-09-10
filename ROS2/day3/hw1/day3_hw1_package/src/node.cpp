#include "day3_hw1_package/node.hpp"

#include <chrono>
#include <cstdio>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

using namespace std::chrono_literals;

TurtlesimControl::TurtlesimControl() : Node("turtlesim_control")
{
    publisher_ = create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    control_timer_ = create_wall_timer(1s, std::bind(&TurtlesimControl::controlLoopCallback, this));
    pen_client_ = create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    keyboard_thread_ = std::thread(&TurtlesimControl::keyboardListenerLoop, this);
}

TurtlesimControl::~TurtlesimControl()
{
    is_running_ = false;
    if (keyboard_thread_.joinable())
        keyboard_thread_.join();
}

void TurtlesimControl::keyboardListenerLoop()
{
    if (!isatty(STDIN_FILENO))
        return;

    termios settings{};
    if (tcgetattr(STDIN_FILENO, &settings) != 0)
        return;

    termios raw_settings = settings;
    raw_settings.c_lflag &= static_cast<unsigned long>(~(ICANON | ECHO));
    raw_settings.c_cc[VMIN] = 0;
    raw_settings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &raw_settings);

    while (is_running_ && rclcpp::ok())
    {
        fd_set input_set;
        FD_ZERO(&input_set);
        FD_SET(STDIN_FILENO, &input_set);
        timeval timeout{0, 100000};

        if (select(STDIN_FILENO + 1, &input_set, nullptr, nullptr, &timeout) > 0)
        {
            char key = '\0';
            if (read(STDIN_FILENO, &key, 1) == 1 && current_mode_ == '\0' &&
                (key == 'w' || key == 'a' || key == 's' || key == 'd'))
            {
                current_mode_ = key;
                step_ = 0;
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSADRAIN, &settings);
}

void TurtlesimControl::controlLoopCallback()
{
    switch (current_mode_.load())
    {
    case 'w':
        circle();
        break;
    case 'a':
        triangle();
        break;
    case 's':
        rectangle();
        break;
    case 'd':
        pentagon();
        break;
    default:
        break;
    }
}

void TurtlesimControl::setPen(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t width,
                              std::uint8_t off)
{
    if (!pen_client_->wait_for_service(100ms))
        return;

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
    request->r = red;
    request->g = green;
    request->b = blue;
    request->width = width;
    request->off = off;
    pen_client_->async_send_request(request);
}

void TurtlesimControl::circle()
{
    if (step_ == 0)
        setPen(0, 255, 0, 4);

    geometry_msgs::msg::Twist message;
    message.linear.x = 2.0;
    message.angular.z = 2.0;
    publisher_->publish(message);
    RCLCPP_INFO(get_logger(), "Publishing: linear.x=%.1f, angular.z=%.1f", message.linear.x, message.angular.z);

    if (step_ > 4)
    {
        publisher_->publish(geometry_msgs::msg::Twist());
        current_mode_ = '\0';
        step_ = 0;
    }
    ++step_;
}

void TurtlesimControl::rectangle()
{
    if (step_ == 0)
        setPen(255, 0, 0, 5);

    geometry_msgs::msg::Twist message;
    if (step_ % 2 == 0)
        message.linear.x = 3.0;
    else
        message.angular.z = 1.5708;
    publisher_->publish(message);
    RCLCPP_INFO(get_logger(), "Publishing: linear.x=%.1f, angular.z=%.4f", message.linear.x, message.angular.z);

    if (step_ >= 7)
    {
        publisher_->publish(geometry_msgs::msg::Twist());
        current_mode_ = '\0';
        step_ = 0;
    }
    ++step_;
}

void TurtlesimControl::triangle()
{
    if (step_ == 0)
        setPen(255, 0, 255, 6);

    geometry_msgs::msg::Twist message;
    if (step_ % 2 == 0)
        message.linear.x = 3.0;
    else
        message.angular.z = 2.094;
    publisher_->publish(message);
    RCLCPP_INFO(get_logger(), "Publishing: linear.x=%.1f, angular.z=%.3f", message.linear.x, message.angular.z);

    if (step_ >= 5)
    {
        publisher_->publish(geometry_msgs::msg::Twist());
        current_mode_ = '\0';
        step_ = 0;
    }
    ++step_;
}

void TurtlesimControl::pentagon()
{
    if (step_ == 0)
        setPen(0, 0, 255, 7);

    geometry_msgs::msg::Twist message;
    if (step_ % 2 == 0)
        message.linear.x = 3.0;
    else
        message.angular.z = 1.2566;
    publisher_->publish(message);
    RCLCPP_INFO(get_logger(), "Publishing: linear.x=%.1f, angular.z=%.4f", message.linear.x, message.angular.z);

    if (step_ >= 9)
    {
        publisher_->publish(geometry_msgs::msg::Twist());
        current_mode_ = '\0';
        step_ = 0;
    }
    ++step_;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<TurtlesimControl>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
