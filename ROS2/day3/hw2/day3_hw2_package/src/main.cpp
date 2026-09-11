#include <QApplication>
#include <rclcpp/rclcpp.hpp>

#include "../include/day3_hw2_package/main_window.hpp"

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    const int result = a.exec();
    rclcpp::shutdown();
    return result;
}
