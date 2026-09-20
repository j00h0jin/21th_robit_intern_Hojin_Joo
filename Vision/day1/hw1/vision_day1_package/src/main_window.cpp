#include "../include/vision_day1_package/main_window.hpp"
#include <QApplication>
#include <iostream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QIcon icon("://ros-icon.png");
    this->setWindowIcon(icon);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}
