#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_wayBtn_clicked();
    void on_setGridBtn_clicked();
    void on_wallBtn_clicked();
    void on_startBtn_clicked();
    void on_endBtn_clicked();
    void on_setBtn_clicked();

private:
    Ui::MainWindow *ui;
};

class MazeWidget : public QWidget {
    Q_OBJECT

public:
    MazeWidget(QWidget *parent = nullptr);
    void setGrid(int N);
    void setMode(int mode) {currentMode = mode;}
    QVector<QVector<int>> getGridData() {return gridData;}

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void handleMouseInput(QMouseEvent *event);
    bool isDrawing = false;
    int lastValue = -1;

    int rows = 10;
    int cols = 10;
    QVector<QVector<int>> gridData; // 0: 길, 1: 벽, 2: 시작, 3: 목표(end)
    int currentMode = 1;
};

#endif // MAINWINDOW_H
