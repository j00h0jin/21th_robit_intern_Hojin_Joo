/**
 * @file /src/vehicle_node.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/day3_hw2_package/vehicle_node.hpp"

#include <chrono>

VehicleNode::VehicleNode() : Node("vehicle_node")
{
    position_publisher_ = create_publisher<geometry_msgs::msg::Pose2D>("topic_vehicle", 10);
    traffic_subscription_ = create_subscription<std_msgs::msg::String>(
        "topic_traffic", 10, std::bind(&VehicleNode::traffic_callback, this, std::placeholders::_1));
    declare_parameter<double>("start_x", 0.0);
    declare_parameter<double>("start_y", 10.0);
    declare_parameter<double>("speed", 1.0);
    declare_parameter<std::string>("direction", "x");
    declare_parameter<double>("stop_line_position", 0.0);
    declare_parameter<double>("road_length", 20.0);
    declare_parameter<double>("acceleration", 0.5);
    declare_parameter<double>("deceleration", 0.5);
    declare_parameter<double>("emergency_deceleration", 2.0);
    declare_parameter<double>("max_speed", 2.0);

    start_x_ = get_parameter("start_x").as_double();
    start_y_ = get_parameter("start_y").as_double();
    direction_ = get_parameter("direction").as_string();
    stop_line_position_ = get_parameter("stop_line_position").as_double();
    road_length_ = get_parameter("road_length").as_double();
    speed_ = get_parameter("speed").as_double();
    acceleration_ = get_parameter("acceleration").as_double();
    deceleration_ = get_parameter("deceleration").as_double();
    emergency_deceleration_ = get_parameter("emergency_deceleration").as_double();
    max_speed_ = get_parameter("max_speed").as_double();

    x_ = get_parameter("start_x").as_double();
    y_ = get_parameter("start_y").as_double();

    current_velocity() = get_parameter("speed").as_double();

    timer_ = create_wall_timer(std::chrono::milliseconds(16), std::bind(&VehicleNode::timer_callback, this));
}

double &VehicleNode::current_velocity()
{
    if (direction_ == "x")
        return vx_;

    return vy_;
}

double &VehicleNode::current_position()
{
    if (direction_ == "x")
        return x_;

    return y_;
}

void VehicleNode::timer_callback()
{
    const double dt = 0.016;
    double &velocity = current_velocity();
    double &position = current_position();

    if (current_light_ == "GREEN")
    {
        velocity += acceleration_ * dt; // v = v_0 + at
        if (velocity > max_speed_)      // 최대 속도인 경우 속도 고정
            velocity = max_speed_;
    }
    else if (current_light_ == "YELLOW")
    {
        const double distance_to_stop_line = stop_line_position_ - position;          // 정지선까지 거리
        const double stopping_distance = velocity * velocity / (2.0 * deceleration_); // 제동 거리(v^2 = 2as 이항)

        // 정지 가능한 경우에 정지, 아니면 그냥 통과
        if (distance_to_stop_line > 0.0 && distance_to_stop_line <= stopping_distance)
        {
            velocity -= deceleration_ * dt;
            if (velocity < 0.0)
                velocity = 0.0;
        }
    }
    else if (current_light_ == "RED")
    {
        // 빨간 불인데 아직 통과 못한 경우
        if (position < stop_line_position_)
        {
            const double distance_to_stop_line = stop_line_position_ - position;                 // 정지선까지 거리
            const double normal_stopping_distance = velocity * velocity / (2.0 * deceleration_); // 일반 제동 거리
            const double emergency_stopping_distance =
                velocity * velocity / (2.0 * emergency_deceleration_); // 급제동 거리

            if (distance_to_stop_line < normal_stopping_distance &&
                distance_to_stop_line >= emergency_stopping_distance)
            {
                velocity -= emergency_deceleration_ * dt;
            }

            if (velocity < 0.0)
                velocity = 0.0;
        }
    }

    const double next_position = position + velocity * dt; // 다음 위치
    // (v^2 = 2as 이항) 급제동 시 정지선 앞에 멈출 수 있는지
    const bool can_stop_at_red_line =
        position < stop_line_position_ && velocity > 0.0 &&
        velocity * velocity / (2.0 * emergency_deceleration_) <= stop_line_position_ - position;

    if (current_light_ == "RED" && can_stop_at_red_line && next_position >= stop_line_position_)
    {
        position = stop_line_position_;
        velocity = 0.0;
    }
    else
    {
        position = next_position;
    }

    // 끝에 도달하면 다시 처음 위치로 보냄(다른 차가 오는 것처럼)
    if (position >= road_length_)
    {
        x_ = start_x_;
        y_ = start_y_;
        position = current_position();
        current_velocity() = speed_;
    }

    geometry_msgs::msg::Pose2D message;
    message.x = x_;
    message.y = y_;
    message.theta = 0.0;
    position_publisher_->publish(message);
}

void VehicleNode::traffic_callback(const std_msgs::msg::String::SharedPtr message)
{
    current_light_ = message->data;
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<VehicleNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
