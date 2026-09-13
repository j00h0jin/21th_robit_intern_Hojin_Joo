/**
 * @file /include/cppqt_project_package/main_window.hpp
 *
 * @brief Qt based gui for %(package)s.
 *
 * @date August 2024
 **/

#ifndef cppqt_project_package_MAIN_WINDOW_H
#define cppqt_project_package_MAIN_WINDOW_H

/*****************************************************************************
** Includes
*****************************************************************************/

#include <QMainWindow>
#include "QIcon"
#include "qnode.hpp"
#include "ui_mainwindow.h"

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
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();
  QNode* qnode;

private:
  Ui::MainWindow* ui;
  void closeEvent(QCloseEvent* event);
};

#endif  // cppqt_project_package_MAIN_WINDOW_H
