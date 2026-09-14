#include "cppqt_project_package/main_window.hpp"
#include "ui_mainwindow.h"
#include <QFont>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      rclcpp::Node("game_gui_node", rclcpp::NodeOptions().automatically_declare_parameters_from_overrides(true)),
      grid_map_(20, 20), is_game_over_(false), survival_time_(0), best_survival_time_(0), stamina_exhausted_(false),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int grid_size = 20;
    this->get_parameter_or("grid_size", grid_size, 20);
    grid_map_ = GridMap(grid_size, grid_size);

    double max_stamina = 120.0;
    this->get_parameter_or("max_stamina", max_stamina, 120.0);

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

    connect(this, &MainWindow::updatePlayerSignal, this, &MainWindow::onPlayerUpdated, Qt::QueuedConnection);
    connect(this, &MainWindow::updateStaminaSignal, this, &MainWindow::onStaminaUpdated, Qt::QueuedConnection);
    connect(this, &MainWindow::updateChaserSignal, this, &MainWindow::onChaserUpdated, Qt::QueuedConnection);

    command_pub_ = this->create_publisher<geometry_msgs::msg::Point>("player_command", 10);

    player_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "player_position", 10, std::bind(&MainWindow::playerCallback, this, std::placeholders::_1));
    stamina_sub_ = this->create_subscription<std_msgs::msg::Float32>(
        "player_stamina", 10, std::bind(&MainWindow::staminaCallback, this, std::placeholders::_1));
    chaser_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "chaser_position", 10, std::bind(&MainWindow::chaserCallback, this, std::placeholders::_1));

    reset_player_client_ = this->create_client<std_srvs::srv::Empty>("reset_player");
    reset_chaser_client_ = this->create_client<std_srvs::srv::Empty>("reset_chaser");
    game_over_client_ = this->create_client<std_srvs::srv::Empty>("game_over_player");

    ros_spin_thread_ = std::thread([this]() { rclcpp::spin(std::shared_ptr<MainWindow>(this, [](MainWindow *) {})); });
    ros_spin_thread_.detach();

    gui_timer_ = new QTimer(this);
    connect(gui_timer_, &QTimer::timeout, this, &MainWindow::updateGraphics);
    gui_timer_->start(50);

    player_move_timer_ = new QTimer(this);
    connect(player_move_timer_, &QTimer::timeout, this, &MainWindow::sendMoveCommand);
    player_move_timer_->start(180);

    game_timer_.start();

    QTimer::singleShot(300, [this]() {
        auto req = std::make_shared<std_srvs::srv::Empty::Request>();
        reset_player_client_->async_send_request(req);
        reset_chaser_client_->async_send_request(req);
    });
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

            auto req = std::make_shared<std_srvs::srv::Empty::Request>();
            game_over_client_->async_send_request(req);

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

    auto req = std::make_shared<std_srvs::srv::Empty::Request>();
    reset_player_client_->async_send_request(req);
    reset_chaser_client_->async_send_request(req);

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
        publishCommand(dx, dy);
    }
}

void MainWindow::publishCommand(double dx, double dy)
{
    geometry_msgs::msg::Point msg;
    msg.x = dx;
    msg.y = dy;
    msg.z = 0.0;
    command_pub_->publish(msg);
}

void MainWindow::playerCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    emit updatePlayerSignal(static_cast<int>(msg->x), static_cast<int>(msg->y), msg->z == 1.0);
}

void MainWindow::staminaCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
    if (is_game_over_)
        return;
    emit updateStaminaSignal(msg->data);
}

void MainWindow::chaserCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    if (is_game_over_)
        return;
    emit updateChaserSignal(static_cast<int>(msg->x), static_cast<int>(msg->y));
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