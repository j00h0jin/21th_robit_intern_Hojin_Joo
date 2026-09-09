/**
 * @file /include/day2_hw2_package/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef day2_hw2_package_QNODE_HPP_
#define day2_hw2_package_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <geometry_msgs/msg/twist.hpp>
#include <rclcpp/rclcpp.hpp>
#include <turtlesim/srv/set_pen.hpp>
#endif
#include <QThread>

/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
    Q_OBJECT
  public:
    QNode();
    ~QNode();

  protected:
    void run();

  private:
    void keyboardListenerLoop();
    void controlLoop();

    void setPen(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t width, std::uint8_t off = 0);

    void circle();
    void triangle();
    void rectangle();
    void pentagon();

    std::shared_ptr<rclcpp::Node> node;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher;

    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr penClient;
    std::atomic<bool> isRunning{true};
    std::atomic<char> currentMode{'\0'};
    std::atomic<int> i{0};
    std::thread keyboardThread;

  Q_SIGNALS:
    void rosShutDown();
};

#endif /* day2_hw2_package_QNODE_HPP_ */
