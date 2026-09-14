#pragma once

#include "cppqt_project_package/grid_map.hpp"
#include "cppqt_project_package/grid_widget.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_srvs/srv/empty.hpp"
#include <QElapsedTimer>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QProgressBar>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <thread>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow, public rclcpp::Node
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  signals:
    void updatePlayerSignal(int x, int y, bool exhausted);
    void updateStaminaSignal(float stamina);
    void updateChaserSignal(int x, int y);

  private slots:
    void updateGraphics();
    void sendMoveCommand();
    void resetGame();

    void onPlayerUpdated(int x, int y, bool exhausted);
    void onStaminaUpdated(float stamina);
    void onChaserUpdated(int x, int y);

  private:
    void playerCallback(const geometry_msgs::msg::Point::SharedPtr msg);
    void staminaCallback(const std_msgs::msg::Float32::SharedPtr msg);
    void chaserCallback(const geometry_msgs::msg::Point::SharedPtr msg);
    void publishCommand(double dx, double dy);

    Ui::MainWindow *ui;

    QTimer *gui_timer_;
    QTimer *player_move_timer_;
    QElapsedTimer game_timer_;
    std::thread ros_spin_thread_;

    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr command_pub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr player_sub_;
    rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr stamina_sub_;
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr chaser_sub_;

    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr reset_player_client_;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr reset_chaser_client_;
    rclcpp::Client<std_srvs::srv::Empty>::SharedPtr game_over_client_;

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