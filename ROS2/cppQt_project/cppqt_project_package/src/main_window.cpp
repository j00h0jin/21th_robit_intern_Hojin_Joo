#include "cppqt_project_package/main_window.hpp"
#include "ui_mainwindow.h"
#include <QFont>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), grid_map_(20, 20), is_game_over_(false), survival_time_(0), best_survival_time_(0),
      stamina_exhausted_(false), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int grid_size = 20;
    ros_connector_.get_parameter_or("grid_size", grid_size, 20);
    grid_map_ = GridMap(grid_size, grid_size);

    double max_stamina = 120.0;
    ros_connector_.get_parameter_or("max_stamina", max_stamina, 120.0);

    QWidget *central_widget = new QWidget(this);
    QHBoxLayout *main_layout = new QHBoxLayout(central_widget);
    main_layout->setContentsMargins(10, 10, 10, 10);

    grid_widget_ = new GridWidget(&grid_map_, &player_x_, &player_y_, &chaser_x_, &chaser_y_, &stamina_exhausted_,
                                  &is_game_over_, this);
    main_layout->addWidget(grid_widget_);
    main_layout->addWidget(ui->rightWidget);

    setCentralWidget(central_widget);
    adjustSize();

    ui->staminaBar->setRange(0, static_cast<int>(max_stamina));
    ui->staminaBar->setValue(static_cast<int>(max_stamina));

    connect(ui->restartBtn, &QPushButton::clicked, this, &MainWindow::resetGame);

    connect(&ros_connector_, &RosConnector::playerUpdated, this, &MainWindow::onPlayerUpdated, Qt::QueuedConnection);
    connect(&ros_connector_, &RosConnector::staminaUpdated, this, &MainWindow::onStaminaUpdated, Qt::QueuedConnection);
    connect(&ros_connector_, &RosConnector::chaserUpdated, this, &MainWindow::onChaserUpdated, Qt::QueuedConnection);

    ros_spin_thread_ =
        std::thread([this]() { rclcpp::spin(std::shared_ptr<RosConnector>(&ros_connector_, [](RosConnector *) {})); });
    ros_spin_thread_.detach();

    gui_timer_ = new QTimer(this);
    connect(gui_timer_, &QTimer::timeout, this, &MainWindow::updateGraphics);
    gui_timer_->start(50);

    player_move_timer_ = new QTimer(this);
    connect(player_move_timer_, &QTimer::timeout, this, &MainWindow::sendMoveCommand);
    player_move_timer_->start(180);

    game_timer_.start();

    QTimer::singleShot(300, [this]() { ros_connector_.requestReset(); });
    grid_widget_->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateGraphics()
{
    if (!is_game_over_)
    {
        survival_time_ = game_timer_.elapsed() / 1000;
        ui->timeLb->setText(QString("time: %1 s").arg(survival_time_));

        if (stamina_exhausted_)
        {
            ui->statusLb->setText("status: EXHAUSTED");
            ui->statusLb->setStyleSheet("color: orange;");
        }
        else
        {
            ui->statusLb->setText("status: Running");
            ui->statusLb->setStyleSheet("color: green;");
        }

        if (player_x_ == chaser_x_ && player_y_ == chaser_y_)
        {
            is_game_over_ = true;
            gui_timer_->stop();
            player_move_timer_->stop();
            ui->statusLb->setText("status: GAME OVER");
            ui->statusLb->setStyleSheet("color: red;");

            ros_connector_.requestGameOver();

            if (survival_time_ > best_survival_time_)
            {
                best_survival_time_ = survival_time_;
                ui->bestLb->setText(QString("best record: %1 s").arg(best_survival_time_));
            }
        }
    }
    grid_widget_->update();
}

void MainWindow::resetGame()
{
    grid_widget_->getInputHandler()->clearKeys();
    is_game_over_ = false;
    game_timer_.restart();
    gui_timer_->start(50);
    player_move_timer_->start(180);
    ui->staminaBar->setValue(ui->staminaBar->maximum());

    ros_connector_.requestReset();
    grid_widget_->setFocus();
}

void MainWindow::sendMoveCommand()
{
    if (is_game_over_)
        return;

    InputHandler *handler = grid_widget_->getInputHandler();
    double dx = 0, dy = 0;
    if (handler->isWPressed())
        dy = -1;
    else if (handler->isSPressed())
        dy = 1;
    else if (handler->isAPressed())
        dx = -1;
    else if (handler->isDPressed())
        dx = 1;

    if (dx != 0 || dy != 0)
    {
        ros_connector_.publishCommand(dx, dy);
    }
}

void MainWindow::onPlayerUpdated(int x, int y, bool exhausted)
{
    player_x_ = x;
    player_y_ = y;
    stamina_exhausted_ = exhausted;
}

void MainWindow::onStaminaUpdated(float stamina)
{
    ui->staminaBar->setValue(static_cast<int>(stamina));
}

void MainWindow::onChaserUpdated(int x, int y)
{
    chaser_x_ = x;
    chaser_y_ = y;
}