#pragma once

#include "QIcon"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QVideoWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
};