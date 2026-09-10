/**
 * @file /include/day2_hw3_package/qsub.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef day2_hw3_package_QSUB_HPP_
#define day2_hw3_package_QSUB_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include "std_msgs/msg/string.hpp"
#include <QString>
#include <QThread>

/*****************************************************************************
** Class
*****************************************************************************/
class QSub : public QThread
{
    Q_OBJECT
  public:
    QSub();
    ~QSub();
    void subString(const std_msgs::msg::String::SharedPtr msg) const;

  protected:
    void run();

  private:
    std::shared_ptr<rclcpp::Node> node;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
    void topic_callback(const std_msgs::msg::String::SharedPtr msg);

  Q_SIGNALS:
    void rosShutDown();
    void receivedString(const QString &msg);
};

#endif /* day2_hw3_package_QSUB_HPP_ */
