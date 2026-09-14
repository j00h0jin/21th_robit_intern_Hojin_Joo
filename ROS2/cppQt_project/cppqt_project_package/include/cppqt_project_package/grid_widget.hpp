#pragma once

#include "cppqt_project_package/grid_map.hpp"
#include "cppqt_project_package/input_handler.hpp"
#include <QKeyEvent>
#include <QPainter>
#include <QWidget>

class GridWidget : public QWidget
{
    Q_OBJECT
  public:
    GridWidget(GridMap *map, int *px, int *py, int *cx, int *cy, bool *exhausted, bool *game_over,
               QWidget *parent = nullptr);

    InputHandler *getInputHandler()
    {
        return &input_handler_;
    }

  protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

  private:
    GridMap *grid_map_;
    int *player_x_;
    int *player_y_;
    int *chaser_x_;
    int *chaser_y_;
    bool *stamina_exhausted_;
    bool *is_game_over_;

    InputHandler input_handler_;
};