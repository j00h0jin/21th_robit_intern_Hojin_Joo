/**
 * @file /include/day3_hw2_package/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef day3_hw2_package_MAIN_WINDOW_H
#define day3_hw2_package_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include "QIcon"
#include "day3_hw2_sub_node.hpp"
#include "ui_mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>

/*****************************************************************************
** Interface [MainWindow]
*****************************************************************************/
/**
 * @brief Qt central, all operations relating to the view part here.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    std::shared_ptr<Day3Hw2SubNode> sub_node_;
    QGraphicsScene *scene_;
    QGraphicsEllipseItem *vehicle_item_;
    QGraphicsEllipseItem *red_light_item_;
    QGraphicsEllipseItem *yellow_light_item_;
    QGraphicsEllipseItem *green_light_item_;
    QTimer *ros_timer_;

    void update_visualization();
    void closeEvent(QCloseEvent *event);
};

#endif // day3_hw2_package_MAIN_WINDOW_H
