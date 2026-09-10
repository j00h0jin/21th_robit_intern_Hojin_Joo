#include "day3_hw1_package/node.hpp"

#include <chrono>
#include <cstdio>
#include <fcntl.h>
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

    config_ = std::make_unique<Parameter>(this);
}

TurtlesimControl::~TurtlesimControl()
{
    is_running_ = false;
    if (keyboard_thread_.joinable())
        keyboard_thread_.join();
}

void TurtlesimControl::keyboardListenerLoop()
{
    int input_fd = open("/dev/tty", O_RDWR);
    if (input_fd < 0)
        input_fd = STDIN_FILENO;

    if (!isatty(input_fd))
    {
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        return;
    }

    termios settings{};
    if (tcgetattr(input_fd, &settings) != 0)
    {
        if (input_fd != STDIN_FILENO)
            close(input_fd);
        return;
    }

    termios raw_settings = settings;
    raw_settings.c_lflag &= static_cast<unsigned long>(~(ICANON | ECHO));
    raw_settings.c_cc[VMIN] = 0;
    raw_settings.c_cc[VTIME] = 0;
    tcsetattr(input_fd, TCSADRAIN, &raw_settings);

    while (is_running_ && rclcpp::ok())
    {
        fd_set input_set;
        FD_ZERO(&input_set);
        FD_SET(input_fd, &input_set);
        timeval timeout{0, 100000};

        if (select(input_fd + 1, &input_set, nullptr, nullptr, &timeout) > 0)
        {
            char key = '\0';
            if (read(input_fd, &key, 1) == 1 && current_mode_ == '\0' &&
                (key == 'w' || key == 'a' || key == 's' || key == 'd'))
            {
                current_mode_ = key;
                step_ = 0;
            }
        }
    }

    tcsetattr(input_fd, TCSADRAIN, &settings);
    if (input_fd != STDIN_FILENO)
        close(input_fd);
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

void TurtlesimControl::setPen(std::uint8_t off)
{
    if (!pen_client_->wait_for_service(100ms))
        return;

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
    request->r = config_->r();
    request->g = config_->g();
    request->b = config_->b();
    request->width = config_->width();
    request->off = off;
    pen_client_->async_send_request(request);
}

void TurtlesimControl::circle()
{
    if (step_ == 0)
        setPen();

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
        setPen();

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
        setPen();

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
        setPen();

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
