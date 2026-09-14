#pragma once

#include "cppqt_project_package/grid_map.hpp"
#include "cppqt_project_package/grid_widget.hpp"
#include "cppqt_project_package/ros_connector.hpp"

#include <QElapsedTimer>
#include <QMainWindow>
#include <QTimer>
#include <QWidget>
#include <thread>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void updateGraphics();
    void sendMoveCommand();
    void resetGame();

    void onPlayerUpdated(int x, int y, bool exhausted);
    void onStaminaUpdated(float stamina);
    void onChaserUpdated(int x, int y);

  private:
    Ui::MainWindow *ui;
    RosConnector ros_connector_;

    QTimer *gui_timer_;
    QTimer *player_move_timer_;
    QElapsedTimer game_timer_;
    std::thread ros_spin_thread_;

    GridMap grid_map_;
    int player_x_;
    int player_y_;
    int chaser_x_;
    int chaser_y_;
    bool is_game_over_;
    int survival_time_;
    int best_survival_time_;
    bool stamina_exhausted_;

    GridWidget *grid_widget_;
};