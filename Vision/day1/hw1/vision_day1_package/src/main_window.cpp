#include "../include/vision_day1_package/main_window.hpp"
#include <QApplication>
#include <iostream>

MainWindow::MainWindow(std::shared_ptr<SubNode> node, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), sub_node(node)
{
    ui->setupUi(this);

    QIcon icon("://ros-icon.png");
    this->setWindowIcon(icon);

    connect(sub_node.get(), &SubNode::imageSignal, this, &MainWindow::updateImage);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(const QImage &img)
{
    if (img.isNull())
        return;

    ui->label->setPixmap(QPixmap::fromImage(img));
}
