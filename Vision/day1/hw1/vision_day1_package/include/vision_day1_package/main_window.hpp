#pragma once

#include "../include/vision_day1_package/ui_node/ui_node.hpp"
#include "QIcon"
#include "ui_mainwindow.h"
#include <QImage>
#include <QMainWindow>
#include <QVideoWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(std::shared_ptr<SubNode> node, QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);

    std::shared_ptr<SubNode> sub_node;

  private slots:
    void updateImage(const QImage &img);
};