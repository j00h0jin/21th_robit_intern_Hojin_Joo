#include "../include/vision_day1_package/main_window.hpp"
#include "rclcpp/rclcpp.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    QApplication a(argc, argv);

    auto sub_node = std::make_shared<SubNode>();
    std::thread ros_thread([sub_node]() { rclcpp::spin(sub_node); });
    ros_thread.detach();

    MainWindow w(sub_node);
    w.show();

    int result = a.exec();
    rclcpp::shutdown();
    return result;
}