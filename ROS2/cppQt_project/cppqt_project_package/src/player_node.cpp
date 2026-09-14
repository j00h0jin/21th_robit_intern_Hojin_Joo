#include "cppqt_project_package/player_node.hpp"

PlayerNode::PlayerNode()
    : Node("player_node"), grid_map_(20, 20), stamina_exhausted_(false), exhaust_counter_(0), game_over_(false)
{
    player_pub_ = this->create_publisher<geometry_msgs::msg::Point>("player_position", 10);
    stamina_pub_ = this->create_publisher<std_msgs::msg::Float32>("player_stamina", 10);

    command_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "player_command", 10, std::bind(&PlayerNode::commandCallback, this, std::placeholders::_1));

    reset_srv_ = this->create_service<std_srvs::srv::Empty>(
        "reset_player", std::bind(&PlayerNode::resetCallback, this, std::placeholders::_1, std::placeholders::_2));
    game_over_srv_ = this->create_service<std_srvs::srv::Empty>(
        "game_over_player", std::bind(&PlayerNode::gameOverCallback, this, std::placeholders::_1,
                                      std::placeholders::_2)); // _1:요청, _2: 응답

    stamina_timer_ =
        this->create_wall_timer(std::chrono::milliseconds(50), std::bind(&PlayerNode::staminaTimerCallback, this));

    this->declare_parameter<int>("grid_size", 20);
    this->declare_parameter<int>("player_start_x", 2);
    this->declare_parameter<int>("player_start_y", 2);
    this->declare_parameter<double>("max_stamina", 120.0);
    this->declare_parameter<double>("stamina_consumption_rate", 3.0);
    this->declare_parameter<double>("stamina_recovery_rate", 2.0);

    grid_size_ = this->get_parameter("grid_size").as_int();
    player_start_x_ = this->get_parameter("player_start_x").as_int();
    player_start_y_ = this->get_parameter("player_start_y").as_int();
    max_stamina_ = this->get_parameter("max_stamina").as_double();
    consumption_rate_ = this->get_parameter("stamina_consumption_rate").as_double();
    recovery_rate_ = this->get_parameter("stamina_recovery_rate").as_double();

    grid_map_ = GridMap(grid_size_, grid_size_);

    player_x_ = player_start_x_;
    player_y_ = player_start_y_;

    wall_stamina_ = max_stamina_;

    geometry_msgs::msg::Point p_msg;
    p_msg.x = player_x_;
    p_msg.y = player_y_;
    p_msg.z = 0.0;
    player_pub_->publish(p_msg);
}

void PlayerNode::resetCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                               std::shared_ptr<std_srvs::srv::Empty::Response> res)
{
    (void)req;
    (void)res;
    player_x_ = player_start_x_;
    player_y_ = player_start_y_;

    wall_stamina_ = max_stamina_;
    stamina_exhausted_ = false;
    exhaust_counter_ = 0;

    game_over_ = false;

    std_msgs::msg::Float32 s_msg;
    s_msg.data = static_cast<float>(wall_stamina_);
    stamina_pub_->publish(s_msg);

    geometry_msgs::msg::Point p_msg;
    p_msg.x = player_x_;
    p_msg.y = player_y_;
    p_msg.z = stamina_exhausted_ ? 1.0 : 0.0; // 사용 안하는 z를 stamina 판별용으로 사용
    player_pub_->publish(p_msg);
}

void PlayerNode::gameOverCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                                  std::shared_ptr<std_srvs::srv::Empty::Response> res)
{
    (void)req;
    (void)res;
    game_over_ = true;
}

// 벽에 붙어있는지 판정
bool PlayerNode::isTouchingWall() const
{
    // 8방향 탐색
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0)
                continue;
            int check_x = player_x_ + dx;
            int check_y = player_y_ + dy;

            // 테두리 벽은 제외
            if (check_x > 0 && check_x < grid_size_ - 1 && check_y > 0 && check_y < grid_size_ - 1)
            {
                // 8방향에 벽이 있는지
                if (grid_map_.getCell(check_x, check_y) == 1)
                    return true;
            }
        }
    }
    return false;
}

void PlayerNode::commandCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    if (!game_over_ && !stamina_exhausted_)
    {
        int next_x = player_x_ + msg->x;
        int next_y = player_y_ + msg->y;

        if (grid_map_.isWalkable(next_x, next_y))
        {
            player_x_ = next_x;
            player_y_ = next_y;
        }
    }

    geometry_msgs::msg::Point p_msg;
    p_msg.x = player_x_;
    p_msg.y = player_y_;
    p_msg.z = stamina_exhausted_ ? 1.0 : 0.0;
    player_pub_->publish(p_msg);
}

void PlayerNode::staminaTimerCallback()
{
    if (game_over_)
        return;

    if (stamina_exhausted_) // 고갈
    {
        exhaust_counter_++;
        if (exhaust_counter_ > 10) // 10 x 50 = 500ms
        {
            stamina_exhausted_ = false;
            exhaust_counter_ = 0;
        }
    }
    else
    {
        // 벽에 붙어있다면 -, 아니면 +
        if (isTouchingWall())
        {
            wall_stamina_ -= consumption_rate_;
            if (wall_stamina_ <= 0.0)
            {
                wall_stamina_ = 0.0;
                stamina_exhausted_ = true; // 고갈
            }
        }
        else
        {
            wall_stamina_ += recovery_rate_;
            if (wall_stamina_ > max_stamina_)
                wall_stamina_ = max_stamina_;
        }
    }

    std_msgs::msg::Float32 s_msg;
    s_msg.data = static_cast<float>(wall_stamina_);
    stamina_pub_->publish(s_msg);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<PlayerNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}