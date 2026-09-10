/**
 * @file /include/day2_hw3_package/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef day2_hw3_package_MAIN_WINDOW_H
#define day2_hw3_package_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include "QIcon"
#include "qpub.hpp"
#include "qsub.hpp"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QPushButton>

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
    QPub *qpub;
    QSub *qsub;

  private slots:
    void on_pubBtn_clicked();

  private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
};

#endif // day2_hw3_package_MAIN_WINDOW_H
