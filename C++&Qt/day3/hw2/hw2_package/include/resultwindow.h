#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPointer>

namespace Ui {
class resultwindow;
}

class resultwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit resultwindow(QVector<QVector<int>> data, QWidget *parent = nullptr);
    ~resultwindow();

private:
    Ui::resultwindow *ui;
};

class Result1Widget : public QWidget {
    Q_OBJECT

public:
    Result1Widget(QWidget *parent = nullptr);
    void setData(QVector<QVector<int>> &data);
    void dijkstra(const QVector<QVector<int>>& grid, int start, int end);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<int>> gridData; // 0: 길, 1: 벽, 2: 시작, 3: 목표(end)
    QVector<QVector<int>> currentGrid; // 시각화용 그리드
    int rows, cols;

};

class Result2Widget : public QWidget {
    Q_OBJECT

public:
    Result2Widget(QWidget *parent = nullptr);
    void setData(QVector<QVector<int>> &data);
    void aStar();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QVector<int>> gridData; // 0: 길, 1: 벽, 2: 시작, 3: 목표(end)

};

#endif // RESULTWINDOW_H
