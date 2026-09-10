/**
 * @file /include/day2_hw3_package/qpub.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef day2_hw3_package_QPUB_HPP_
#define day2_hw3_package_QPUB_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include "std_msgs/msg/string.hpp"
#include <QThread>

/*****************************************************************************
** Class
*****************************************************************************/
class QPub : public QThread
{
    Q_OBJECT
  public:
    QPub();
    ~QPub();
    void pubString(QString msg);

  protected:
    void run();

  private:
    std::shared_ptr<rclcpp::Node> node;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;

  Q_SIGNALS:
    void rosShutDown();
};

#endif /* day2_hw3_package_QPUB_HPP_ */
