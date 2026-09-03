#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_forward_pushBtn_clicked()
{
    current_state = "forward";
    ui -> status_label -> setText(current_state);
}


void MainWindow::on_backward_pushBtn_clicked()
{
    current_state = "backward";
    ui -> status_label -> setText(current_state);
}


void MainWindow::on_left_pushBtn_clicked()
{
    current_state = "left";
    ui -> status_label -> setText(current_state);
}


void MainWindow::on_right_pushBtn_clicked()
{
    current_state = "right";
    ui -> status_label -> setText(current_state);
}


void MainWindow::on_stop_pushBtn_clicked()
{
    current_state = "stop";
    ui -> status_label -> setText(current_state);
}


void MainWindow::on_speed_slider_valueChanged(int value)
{
    speed = value;
    QString temp = QString::number(speed);
    ui -> speed_label -> setText(temp);
}

