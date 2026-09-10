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

#include "../include/day2_hw3_package/main_window.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QPushButton *pubBtn;
    // QPlainTextEdit *pubPlain;

    // QPushButton *subBtn;
    // QPlainTextEdit *subPlain;

    QIcon icon("://ros-icon.png");
    this->setWindowIcon(icon);

    qpub = new QPub();
    QObject::connect(qpub, &QPub::rosShutDown, this, &MainWindow::close);

    qsub = new QSub();
    QObject::connect(qsub, &QSub::rosShutDown, this, &MainWindow::close);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMainWindow::closeEvent(event);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pubBtn_clicked()
{
    QString msg = ui->pubPlain->toPlainText();
    qpub->pubString(msg);

    ui->pubPlain->clear(); // 입력창 비우기
}
