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

#include "../include/day2_hw2_package/qnode.hpp"

#include <atomic>
#include <chrono>
#include <cstdint>
#include <sys/select.h>
#include <termios.h>
#include <thread>
#include <unistd.h>

QNode::QNode()
{
    int argc = 0;
    char **argv = NULL;
    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("day2_hw2_package");
    velSub = node->create_subscription<geometry_msgs::msg::Twist>(
        "/turtle1/cmd_vel", 10, std::bind(&QNode::velCallback, this, std::placeholders::_1));

    publisher = node->create_publisher<geometry_msgs::msg::Twist>("turtle1/cmd_vel", 10);
    penClient = node->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
    keyboardThread = std::thread(&QNode::keyboardListenerLoop, this);
    this->start(); // start -> run() 실행
}

QNode::~QNode()
{
    isRunning = false;
    if (keyboardThread.joinable()) // thread 종료 대기
        keyboardThread.join();

    if (rclcpp::ok())
    {
        rclcpp::shutdown();
    }

    wait(); // run() 종료 대기
}

void QNode::run()
{
    rclcpp::WallRate loop_rate(20);

    auto lastControl = std::chrono::steady_clock::now();
    while (rclcpp::ok()) // ok(): ros2 실행 여부, shutdown 시 false
    {
        rclcpp::spin_some(node); // ros2에서 처리해야 할 콜백 처리
        const auto now = std::chrono::steady_clock::now();
        if (now - lastControl >= std::chrono::seconds(1)) // loop 1s마다 실행
        {
            controlLoop();
            lastControl = now;
        }
        loop_rate.sleep(); // 과연산 방지
    }
    rclcpp::shutdown();
    Q_EMIT rosShutDown();
}

void QNode::velCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    linearXValue.store(msg->linear.x);
    angularZValue.store(msg->angular.z);
}

void QNode::keyboardListenerLoop()
{
    if (!isatty(STDIN_FILENO)) // 터미널인지
        return;

    termios settings{};
    if (tcgetattr(STDIN_FILENO, &settings) != 0) // 터미널 설정 받아오기
        return;

    termios rawSettings = settings;
    rawSettings.c_lflag &= static_cast<unsigned long>(~(ICANON | ECHO)); // 즉시 입력 받기, 문자 출력X
    rawSettings.c_cc[VMIN] = 0;
    rawSettings.c_cc[VTIME] = 0;
    tcsetattr(STDIN_FILENO, TCSADRAIN, &rawSettings);

    while (isRunning && rclcpp::ok())
    {
        fd_set inputSet;
        FD_ZERO(&inputSet);
        FD_SET(STDIN_FILENO, &inputSet);
        timeval timeout{0, 100000}; // s, us = 0.1s
        if (select(STDIN_FILENO + 1, &inputSet, nullptr, nullptr, &timeout) > 0)
        {
            // 입력이 있는 경우
            char key = '\0'; // null
            // 키 입력 && 그리는 중 X && wasd중 하나인 경우
            if (read(STDIN_FILENO, &key, 1) == 1 && currentMode == '\0' &&
                (key == 'w' || key == 'a' || key == 's' || key == 'd'))
            {
                currentMode = key;
                i = 0;
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSADRAIN, &settings);
}

void QNode::controlLoop()
{
    switch (currentMode.load())
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

void QNode::setPen(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t width, std::uint8_t off)
{
    if (!penClient->wait_for_service(std::chrono::milliseconds(100)))
        return;

    auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
    request->r = red;
    request->g = green;
    request->b = blue;
    request->width = width;
    request->off = off;
    penClient->async_send_request(request);
}

void QNode::circle()
{
    if (i == 0)
        setPen(0, 255, 0, 4);

    geometry_msgs::msg::Twist msg;
    msg.linear.x = 2.0;
    msg.angular.z = 2.0;
    publisher->publish(msg);
    if (i > 4)
    {
        publisher->publish(geometry_msgs::msg::Twist());
        currentMode = '\0'; // null char
        i = 0;
    }
    i += 1;
}

void QNode::rectangle()
{
    if (i == 0)
        setPen(255, 0, 0, 5);

    geometry_msgs::msg::Twist msg;
    if (i % 2 == 0)
        msg.linear.x = 3.0;
    else
        msg.angular.z = 1.5708;

    publisher->publish(msg);
    if (i >= 7)
    {
        publisher->publish(geometry_msgs::msg::Twist());
        currentMode = '\0';
        i = 0;
    }
    i += 1;
}

void QNode::triangle()
{
    if (i == 0)
        setPen(255, 0, 255, 6);
    geometry_msgs::msg::Twist msg;
    if (i % 2 == 0)
        msg.linear.x = 3.0;
    else
        msg.angular.z = 2.094;
    publisher->publish(msg);
    if (i >= 5)
    {
        publisher->publish(geometry_msgs::msg::Twist());
        currentMode = '\0';
        i = 0;
    }
    i += 1;
}

void QNode::pentagon()
{
    if (i == 0)
        setPen(0, 0, 255, 7);

    geometry_msgs::msg::Twist msg;
    if (i % 2 == 0)
        msg.linear.x = 3.0;
    else
        msg.angular.z = 1.2566;

    publisher->publish(msg);
    if (i >= 9)
    {
        publisher->publish(geometry_msgs::msg::Twist());
        currentMode = '\0';
        i = 0;
    }
    i += 1;
}
