/**
 * @file /src/main_window.cpp
 *
 * @brief Implementation for the qt gui.
 *
 * @date August 2024
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/main_window.hpp"

#include <QBrush>
#include <QColor>
#include <QPen>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon("://ros-icon.png");
    this->setWindowIcon(icon);

    scene_ = new QGraphicsScene(this);
    scene_->setSceneRect(0.0, 0.0, 500.0, 500.0);
    ui->graphicsView->setScene(scene_);

    scene_->setBackgroundBrush(QColor("#6f9f63"));

    // Simple top-down four-way intersection.
    // scene_->addRect(0.0, 175.0, 500.0, 150.0, QPen(Qt::NoPen), QBrush(QColor("#3f4448")));
    scene_->addRect(0.0, 240, 500.0, 20.0, QPen(Qt::NoPen), QBrush(QColor("#3f4448")));
    scene_->addRect(260.0, 0.0, 20.0, 500.0, QPen(Qt::NoPen), QBrush(QColor("#3f4448")));

    red_light_item_ = scene_->addEllipse(20.0, 20.0, 30.0, 30.0, QPen(Qt::black), QBrush(Qt::gray));
    yellow_light_item_ = scene_->addEllipse(60.0, 20.0, 30.0, 30.0, QPen(Qt::black), QBrush(Qt::gray));
    green_light_item_ = scene_->addEllipse(100.0, 20.0, 30.0, 30.0, QPen(Qt::black), QBrush(Qt::gray));
    vehicle_item_ = scene_->addEllipse(0.0, 0.0, 28.0, 28.0, QPen(Qt::black), QBrush(Qt::blue));

    sub_node_ = std::make_shared<Day3Hw2SubNode>();
    ros_timer_ = new QTimer(this);
    connect(ros_timer_, &QTimer::timeout, this, [this]() {
        rclcpp::spin_some(sub_node_);
        update_visualization();
    });
    ros_timer_->start(16);
}

void MainWindow::update_visualization()
{
    const std::string state = sub_node_->traffic_state();
    red_light_item_->setBrush(QBrush(state == "RED" ? Qt::red : Qt::gray));
    yellow_light_item_->setBrush(QBrush(state == "YELLOW" ? Qt::yellow : Qt::gray));
    green_light_item_->setBrush(QBrush(state == "GREEN" ? Qt::green : Qt::gray));

    const auto position = sub_node_->vehicle_position();
    constexpr double world_scale = 40.0;
    const double scene_x = 50.0 + position.x * world_scale;
    const double scene_y = 450.0 - position.y * world_scale;
    vehicle_item_->setPos(scene_x - 14.0, scene_y - 14.0);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    ros_timer_->stop();
    sub_node_.reset();
    delete ui;
}
