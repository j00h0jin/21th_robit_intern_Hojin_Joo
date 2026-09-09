#include <QApplication>
#include <iostream>

#include "../include/day2_hw1_package/main_window.hpp"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  MainWindow w;
  w.show();
  return a.exec();
}
