/**
 * @file /include/day3_hw2_package/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef day3_hw2_package_QNODE_HPP_
#define day3_hw2_package_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
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
  std::shared_ptr<rclcpp::Node> node;

Q_SIGNALS:
  void rosShutDown();
};

#endif /* day3_hw2_package_QNODE_HPP_ */
