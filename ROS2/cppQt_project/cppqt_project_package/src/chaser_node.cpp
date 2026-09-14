#include "cppqt_project_package/chaser_node.hpp"
#include <cmath>
#include <queue>
#include <utility>
#include <vector>

ChaserNode::ChaserNode() : Node("chaser_node"), grid_map_(20, 20), last_dx_(0), last_dy_(0), is_turning_delay_(false)
{
    chaser_pub_ = this->create_publisher<geometry_msgs::msg::Point>("chaser_position", 10);

    player_pos_sub_ = this->create_subscription<geometry_msgs::msg::Point>(
        "player_position", 10, std::bind(&ChaserNode::playerPosCallback, this, std::placeholders::_1));

    reset_srv_ = this->create_service<std_srvs::srv::Empty>(
        "reset_chaser", std::bind(&ChaserNode::resetCallback, this, std::placeholders::_1, std::placeholders::_2));

    this->declare_parameter<int>("chaser_update_interval_ms", 150);
    int interval_ms = this->get_parameter("chaser_update_interval_ms").as_int();
    timer_ =
        this->create_wall_timer(std::chrono::milliseconds(interval_ms), std::bind(&ChaserNode::updateChaser, this));

    this->declare_parameter<int>("grid_size", 20);
    this->declare_parameter<int>("chaser_start_x", 18);
    this->declare_parameter<int>("chaser_start_y", 18);
    this->declare_parameter<double>("chaser_momentum_factor", 1.5);

    grid_size_ = this->get_parameter("grid_size").as_int();
    chaser_start_x_ = this->get_parameter("chaser_start_x").as_int();
    chaser_start_y_ = this->get_parameter("chaser_start_y").as_int();

    grid_map_ = GridMap(grid_size_, grid_size_);
    chaser_x_ = chaser_start_x_;
    chaser_y_ = chaser_start_y_;

    player_x_ = -1.0;
    player_y_ = -1.0;
}

void ChaserNode::playerPosCallback(const geometry_msgs::msg::Point::SharedPtr msg)
{
    player_x_ = msg->x;
    player_y_ = msg->y;
}

void ChaserNode::resetCallback(const std::shared_ptr<std_srvs::srv::Empty::Request> req,
                               std::shared_ptr<std_srvs::srv::Empty::Response> res)
{
    (void)req;
    (void)res;

    // reset
    chaser_x_ = chaser_start_x_;
    chaser_y_ = chaser_start_y_;
    last_dx_ = 0;
    last_dy_ = 0;
    is_turning_delay_ = false;

    geometry_msgs::msg::Point c_msg;
    c_msg.x = chaser_x_;
    c_msg.y = chaser_y_;
    c_msg.z = 0.0;
    chaser_pub_->publish(c_msg);
}

bool ChaserNode::isPathClear(int x1, int y1, int x2, int y2)
{
    if (x1 == x2)
    {
        int start_y = std::min(y1, y2);
        int end_y = std::max(y1, y2);
        for (int y = start_y + 1; y < end_y; y++)
        // 두 y좌표 사이에 막힌 좌표가 있는지 확인
        {
            if (!grid_map_.isWalkable(x1, y))
                return false;
        }
        return true;
    }
    if (y1 == y2)
    {
        int start_x = std::min(x1, x2);
        int end_x = std::max(x1, x2);
        for (int x = start_x + 1; x < end_x; x++)
        {
            // 두 x좌표 사이에 막힌 좌표가 있는지 확인
            if (!grid_map_.isWalkable(x, y1))
                return false;
        }
        return true;
    }
    return false;
}

void ChaserNode::updateChaser()
{
    // 맨해튼 거리
    int distance = std::abs(chaser_x_ - player_x_) + std::abs(chaser_y_ - player_y_);

    singleStep();
    // 거리가 멀다면 한번 더 움직이도록
    if (distance >= 6)
        singleStep();

    geometry_msgs::msg::Point c_msg;
    c_msg.x = chaser_x_;
    c_msg.y = chaser_y_;
    c_msg.z = 0.0;
    chaser_pub_->publish(c_msg);
}

void ChaserNode::singleStep()
{
    int target_x = player_x_;
    int target_y = player_y_;

    if (chaser_x_ == target_x && chaser_y_ == target_y)
        return; // 잡힘

    int next_x = chaser_x_;
    int next_y = chaser_y_;

    // 다음 경로 안막힌 경우(벽 x)
    if (isPathClear(chaser_x_, chaser_y_, target_x, target_y))
    {
        if (chaser_x_ < target_x)
            next_x++;
        else if (chaser_x_ > target_x)
            next_x--;
        else if (chaser_y_ < target_y)
            next_y++;
        else if (chaser_y_ > target_y)
            next_y--;

        // 해당 좌표 이동 가능하면 이동
        if (grid_map_.isWalkable(next_x, next_y))
        {
            chaser_x_ = next_x;
            chaser_y_ = next_y;
            return;
        }
    }

    // BFS
    std::vector<std::vector<bool>> visited(grid_size_, std::vector<bool>(grid_size_, false));
    // 경로 저장 parent
    std::vector<std::vector<std::pair<int, int>>> parent(grid_size_,
                                                         std::vector<std::pair<int, int>>(grid_size_, {-1, -1}));
    std::queue<std::pair<int, int>> q;

    q.push({chaser_x_, chaser_y_});
    visited[chaser_y_][chaser_x_] = true;

    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, 1, 0, -1};
    bool found = false;

    // 큐 빌 때까지
    while (!q.empty())
    {
        auto [cx, cy] = q.front();
        q.pop();

        if (cx == target_x && cy == target_y) // 도달
        {
            found = true;
            break;
        }

        for (int i = 0; i < 4; i++) // 맨해튼 거리이므로 4방향
        {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            // 방문 가능(벽x)이면서 방문하지 않은 경우
            if (grid_map_.isWalkable(nx, ny) && !visited[ny][nx])
            {
                visited[ny][nx] = true;
                parent[ny][nx] = {cx, cy};
                q.push({nx, ny});
            }
        }
    }

    if (found)
    {
        int curr_x = target_x;
        int curr_y = target_y;

        // 현재 좌표 직전까지 while
        while (parent[curr_y][curr_x] != std::pair<int, int>{chaser_x_, chaser_y_})
        {
            auto p = parent[curr_y][curr_x];
            if (p.first == -1 && p.second == -1) // 경로가 없는 경우
                break;
            curr_x = p.first;
            curr_y = p.second;
        }

        int next_dx = curr_x - chaser_x_;
        int next_dy = curr_y - chaser_y_;

        // 기본적으로 chaser 이속 > player 이속
        // 꺾 딜레이로 아슬한 컨 제작

        // 방향이 다르면
        if ((next_dx != last_dx_ || next_dy != last_dy_) && (last_dx_ != 0 || last_dy_ != 0))
        {
            if (!is_turning_delay_)
            // 회전 딜레이
            {
                is_turning_delay_ = true;
                return;
            }
        }

        // 같으면 그냥 진행
        is_turning_delay_ = false; // 이전에 꺾은 경우 다시 false로 변경
        last_dx_ = next_dx;
        last_dy_ = next_dy;
        chaser_x_ = curr_x;
        chaser_y_ = curr_y;
    }
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ChaserNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}