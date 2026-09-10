#include <QApplication>
#include <iostream>

#include "../include/day2_hw3_package/main_window.hpp"

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    const int result = a.exec();
    if (rclcpp::ok())
        rclcpp::shutdown();
    return result;
}
