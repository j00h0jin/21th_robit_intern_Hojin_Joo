#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "resultwindow.h"
#include <QPushButton>
#include <QMouseEvent>
#include <QPainter>

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

MazeWidget::MazeWidget(QWidget *parent) : QWidget(parent) {
    gridData.resize(rows, QVector<int>(cols, 0));
    setAttribute(Qt::WA_StyledBackground, true);
    setMouseTracking(true);
}

void MazeWidget::setGrid(int N) {
    rows = N;
    cols = N;
    gridData.resize(rows);

    for (int i = 0; i < rows; ++i) {
        gridData[i].resize(cols, 0);
    }
    update();
}

void MazeWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    int cellWidth = width() / cols;
    int cellHeight = height() / rows;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // 셀의 상태에 따라 색상 지정
            if(gridData[r][c] == 0) {
                painter.setBrush(Qt::white);
            }
            else if (gridData[r][c] == 1) {
                painter.setBrush(Qt::black); // 벽
            }
            else if (gridData[r][c] == 2) {
                painter.setBrush(Qt::red); // 시작점
            }
            else if (gridData[r][c] == 3) {
                painter.setBrush(Qt::blue); // 도착점
            }

            painter.setPen(Qt::gray); // 격자 색상
            painter.drawRect(c * cellWidth, r * cellHeight, cellWidth, cellHeight);
        }
    }
}

void MazeWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = true; // 그리기 시작
        handleMouseInput(event);
    }
}

void MazeWidget::mouseMoveEvent(QMouseEvent *event) {
    // 마우스 왼쪽 버튼이 눌린 상태에서 움직일 때만 실행
    if (isDrawing && (event->buttons() & Qt::LeftButton)) {
        handleMouseInput(event); // 이동 중인 칸 처리
    }
}

void MazeWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDrawing = false; // 그리기 종료
        lastValue = -1;
    }
}

void MazeWidget::handleMouseInput(QMouseEvent *event) {
    int cellWidth = width() / cols;
    int cellHeight = height() / rows;

    if (rows <= 0 || cols <= 0 || cellWidth <= 0 || cellHeight <= 0) return;

    // 클릭/이동한 좌표 계산
    int c = event->position().x() / cellWidth;
    int r = event->position().y() / cellHeight;

    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        if (currentMode == 2 || currentMode == 3) {
            for (int i = 0; i < rows; ++i) {
                for (int j = 0; j < cols; ++j) {
                    if (gridData[i][j] == currentMode) {
                        gridData[i][j] = 0; // 기존 위치를 길로 변경
                    }
                }
            }
        }

        // 계산된 칸에 저장된 값을 적용
        gridData[r][c] = currentMode;
        update();
    }
}

void MainWindow::on_setGridBtn_clicked()
{
    int value = ui -> spinBox -> value();
    ui->widget->setGrid(value);
    ui->widget->update();
}

void MainWindow::on_wayBtn_clicked()
{
    ui->widget->setMode(0);
}


void MainWindow::on_wallBtn_clicked()
{
    ui->widget->setMode(1);
}


void MainWindow::on_startBtn_clicked()
{
    ui->widget->setMode(2);
}


void MainWindow::on_endBtn_clicked()
{
    ui->widget->setMode(3);
}


void MainWindow::on_setBtn_clicked()
{
    // 조건문 추가하기 -> start, end가 반드시 존재해야 함
    QVector<QVector<int>> mazeData = ui->widget->getGridData();

    resultwindow *resultWindow = new resultwindow(mazeData, this);
    resultWindow->setAttribute(Qt::WA_DeleteOnClose); // 창 닫힐 때 메모리 자동 해제
    resultWindow->show();

    // 메인 창 비활성화
    this->hide();
}
