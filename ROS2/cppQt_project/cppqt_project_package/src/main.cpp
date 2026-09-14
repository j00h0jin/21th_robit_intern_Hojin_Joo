#include "cppqt_project_package/main_window.hpp"
#include "rclcpp/rclcpp.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    QApplication app(argc, argv);
    MainWindow w;
    w.show();

    int result = app.exec();

    rclcpp::shutdown();
    return result;
}