#include "cppqt_project_package/grid_widget.hpp"
#include <QFont>

GridWidget::GridWidget(GridMap *map, int *px, int *py, int *cx, int *cy, bool *exhausted, bool *game_over,
                       QWidget *parent)
    : QWidget(parent), grid_map_(map), player_x_(px), player_y_(py), chaser_x_(cx), chaser_y_(cy),
      stamina_exhausted_(exhausted), is_game_over_(game_over)
{
    int cell_size = 50;
    setFixedSize(grid_map_->getWidth() * cell_size, grid_map_->getHeight() * cell_size);
    setFocusPolicy(Qt::StrongFocus);
}

void GridWidget::keyPressEvent(QKeyEvent *event)
{
    input_handler_.handleKeyPress(event, *is_game_over_, *stamina_exhausted_);
}

void GridWidget::keyReleaseEvent(QKeyEvent *event)
{
    input_handler_.handleKeyRelease(event);
}

void GridWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    int cell_size = 50;

    for (int y = 0; y < grid_map_->getHeight(); ++y)
    {
        for (int x = 0; x < grid_map_->getWidth(); ++x)
        {
            if (grid_map_->getCell(x, y) == 1)
                painter.setBrush(Qt::darkGray);
            else
                painter.setBrush(Qt::white);

            painter.setPen(Qt::lightGray);
            painter.drawRect(x * cell_size, y * cell_size, cell_size, cell_size);
        }
    }

    if (*stamina_exhausted_)
        painter.setBrush(Qt::yellow);
    else
        painter.setBrush(Qt::blue);

    painter.drawRect((*player_x_) * cell_size + 4, (*player_y_) * cell_size + 4, cell_size - 8, cell_size - 8);

    painter.setBrush(Qt::red);
    painter.drawRect((*chaser_x_) * cell_size + 4, (*chaser_y_) * cell_size + 4, cell_size - 8, cell_size - 8);

    if (*is_game_over_)
    {
        painter.setBrush(QColor(0, 0, 0, 160));
        painter.drawRect(rect());
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 28, QFont::Bold));
        painter.drawText(rect(), Qt::AlignCenter, "GAME OVER");
    }
}