#include "resultwindow.h"
#include "./ui_resultwindow.h"
#include <QPainter>
#include <queue>
#include <vector>
#include <QPointer>
#include <QThread>
#include <QCoreApplication>
#include <QTimer>
#define INF 1000000000

using namespace std;

resultwindow::resultwindow(QVector<QVector<int>> data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::resultwindow)
{
    ui->setupUi(this);
    ui->widget1->setData(data);
    ui->widget2->setData(data);
}

resultwindow::~resultwindow()
{
    delete ui;
}

Result1Widget::Result1Widget(QWidget *parent) : QWidget(parent) {}

void Result1Widget::setData(QVector<QVector<int>> &data) {
    gridData = data;
    currentGrid = data;
    update();

    QTimer::singleShot(100, this, [=]() {
        int start = -1;
        int end = -1;
        int rows = gridData.size();
        if (rows == 0) return;
        int cols = gridData[0].size();

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (gridData[r][c] == 2) start = r * cols + c;
                if (gridData[r][c] == 3) end = r * cols + c;
            }
        }

        // start나 end 미지정시 반응 X
        if (start == -1 || end == -1) {
            return;
        }

        dijkstra(gridData, start, end);
    });
}

void Result1Widget::dijkstra(const QVector<QVector<int>>& grid, int start, int end) {
    currentGrid = grid;

    int rows = gridData.size();
    int cols = gridData[0].size();
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int totalNodes = rows*cols;

    std::vector<std::vector<std::pair<int, int>>> nodes(totalNodes);
    std::vector<int> dist(totalNodes, INF);
    std::vector<int> parent(totalNodes, -1); // 경로 역추적용

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (gridData[r][c] == 1) continue; // 벽은 무시

            int u = r * cols + c;

            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                    if (gridData[nr][nc] != 1) { // 이동 가능한 길이면 간선 추가
                        int v = nr * cols + nc;
                        nodes[u].push_back({1, v}); // 비용 1, 연결된 노드 v
                    }
                }
            }
        }
    }

    std::priority_queue<std::pair<int, int>> pq;

    pq.push({0, start});
    dist[start] = 0;

    while (!pq.empty()) {
        int cost = -pq.top().first;
        int here = pq.top().second;
        pq.pop();

        if (dist[here] < cost) continue;
        int r = here / cols;
        int c = here % cols;
        if (currentGrid[r][c] != 2 && currentGrid[r][c] != 3) {
            currentGrid[r][c] = 4; // 탐색된 영역
            update();
            QCoreApplication::processEvents();
            QThread::msleep(50);
        }
        if (here == end) break; // 목적지 도달 시 조기 종료

        for (size_t i = 0; i < nodes[here].size(); i++) {
            int via_cost = cost + nodes[here][i].first;
            int nextNode = nodes[here][i].second;

            if (via_cost < dist[nextNode]) {
                dist[nextNode] = via_cost;
                parent[nextNode] = here; // 부모 기록
                pq.push({-via_cost, nextNode});
            }
        }
    }
    int curr = end;
    while (curr != -1) {
        int r = curr / cols;
        int c = curr % cols;

        if (currentGrid[r][c] != 2 && currentGrid[r][c] != 3) {
            currentGrid[r][c] = 5; // 최단 경로 영역
        }
        curr = parent[curr];

    }

    update();

}

void Result1Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (currentGrid.isEmpty()) return;

    int rows = currentGrid.size();
    int cols = currentGrid[0].size();

    // 위젯 크기를 행/열 개수로 나누어 각 칸의 너비와 높이 계산
    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            // 셀의 상태에 따라 색상 지정
            if(currentGrid[r][c] == 0) {
                painter.setBrush(Qt::white);
            }
            else if (currentGrid[r][c] == 1) {
                painter.setBrush(Qt::black); // 벽
            }
            else if (currentGrid[r][c] == 2) {
                painter.setBrush(Qt::red); // 시작점
            }
            else if (currentGrid[r][c] == 3) {
                painter.setBrush(Qt::blue); // 도착점
            }
            else if (currentGrid[r][c] == 4) {
                painter.setBrush(QColor(173, 216, 230)); // 탐색 구간
            }
            else if (currentGrid[r][c] == 5) {
                painter.setBrush(QColor(255, 165, 0));   // 최단 경로
            }

            painter.setPen(Qt::gray); // 격자 색상
            painter.drawRect(c * cellWidth, r * cellHeight, cellWidth, cellHeight);
        }
    }
}

Result2Widget::Result2Widget(QWidget *parent) : QWidget(parent) {}

void Result2Widget::setData(QVector<QVector<int>> &data) {
    gridData = data;
    update();
    aStar();
}

void Result2Widget::aStar() {

}

void Result2Widget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    if (gridData.isEmpty()) return;

    int rows = gridData.size();
    int cols = gridData[0].size();

    // 위젯 크기를 행/열 개수로 나누어 각 칸의 너비와 높이 계산
    double cellWidth = static_cast<double>(width()) / cols;
    double cellHeight = static_cast<double>(height()) / rows;

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
