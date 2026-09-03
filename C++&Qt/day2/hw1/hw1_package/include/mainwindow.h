#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
   QString current_state; // forward, backward, left, right, stop
    int speed;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_forward_pushBtn_clicked();

    void on_backward_pushBtn_clicked();

    void on_left_pushBtn_clicked();

    void on_right_pushBtn_clicked();

    void on_stop_pushBtn_clicked();

    void on_speed_slider_valueChanged(int value);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
