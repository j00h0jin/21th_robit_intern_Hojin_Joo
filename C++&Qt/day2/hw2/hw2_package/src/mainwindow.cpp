#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *Widget = new QWidget(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(Widget); // H: horizon

    QVBoxLayout *buttonLayout = new QVBoxLayout(); // V: vertical

    QPushButton *undoButton = new QPushButton("Undo", this);
    QPushButton *redoButton = new QPushButton("Redo", this);

    buttonLayout->addWidget(undoButton);
    buttonLayout->addWidget(redoButton);
    buttonLayout->addStretch(); // 빈 공간을 채움
    // buttonLayout에 vertical로 undoBtn, redoBtn을 배치(stretch 포함)

    ScribbleArea *scribbleArea = new ScribbleArea(this);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(scribbleArea, 1);
    // mainLayout에 horizon으로 버튼과 scribble 화면을 배치

    setCentralWidget(Widget);

    // button의 cliked signal을 각각 undo, redo 함수와 연결)
    connect(undoButton, &QPushButton::clicked, scribbleArea, &ScribbleArea::undo);
    connect(redoButton, &QPushButton::clicked, scribbleArea, &ScribbleArea::redo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

ScribbleArea::ScribbleArea(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_StaticContents);
    scribbling = false; // true: 마우스가 눌려 있는 상태, false: 그 반대
    myPenWidth = 3; // 두께
    myPenColor = Qt::black; // 색
    image.fill(qRgb(255, 255, 255)); // 흰색으로 fill
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
    // 좌클릭 event가 감지되면(눌림)
    if (event->button() == Qt::LeftButton) {
        cacheImage = image;
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    // 그리는 중
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    //  좌클릭 event가 감지되면(떼짐)
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;

        undoStack.push_back(cacheImage);
        redoStack.clear();
    }
}

// 변경된 화면(event)을 적용
void ScribbleArea::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

// 창 크기가 변경(event)되면 창에 맞춰서 resize하게 됨
void ScribbleArea::resizeEvent(QResizeEvent *event)
{
    if (width() > image.width() || height() > image.height()) {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }
    QWidget::resizeEvent(event);
}

void ScribbleArea::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);

    int rad = (myPenWidth / 2) + 2;
    update(QRect(lastPoint, endPoint).normalized()
               .adjusted(-rad, -rad, +rad, +rad));
    lastPoint = endPoint;
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void ScribbleArea::undo()
{
    if (undoStack.empty()) // 비어있으면 무시
        return;

    redoStack.push_back(image); // redo stack에 push(redo용)
    image = undoStack.back(); // 이전 image(back)를 image에 저장
    undoStack.pop_back(); // pop

    update();
}

void ScribbleArea::redo()
{
    if (redoStack.empty())
        return;

    undoStack.push_back(image);
    image = redoStack.back();
    redoStack.pop_back();

    update();
}


