#include "../include/day4_hw2_package/subscriber_node.hpp"

Day4Hw2Subscriber::Day4Hw2Subscriber(const std::string &node_name) : Node(node_name)
{
    // lc_talker 노드로부터 오는 데이터 토픽
    sub_data_ = this->create_subscription<std_msgs::msg::String>(
        "lc_topic", 10, [this](std_msgs::msg::String::ConstSharedPtr msg) { return this->data_callback(msg); });

    // 알림 이벤트 토픽. 모든 상태 변화는 전환을 나타내는 시작 상태와 목표 상태를 가진 TransitionEvent로 여기에
    // 퍼블리시됩니다.
    sub_notification_ = this->create_subscription<lifecycle_msgs::msg::TransitionEvent>(
        "/lc_pub/transition_event", 10,
        [this](lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg) { return this->notification_callback(msg); });
}

void Day4Hw2Subscriber::data_callback(std_msgs::msg::String::ConstSharedPtr msg)
{
    RCLCPP_INFO(get_logger(), "data_callback: %s", msg->data.c_str());
}

void Day4Hw2Subscriber::notification_callback(lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg)
{
    RCLCPP_INFO(get_logger(), "notify callback: Transition from state %s to %s", msg->start_state.label.c_str(),
                msg->goal_state.label.c_str());
}

int main(int argc, char **argv)
{
    // stdout 버퍼를 강제로 플러시합니다.
    // 런치 파일 내에서 동시에 실행될 때도 모든 출력이 올바르게 동기화되도록 보장합니다.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    auto lc_listener = std::make_shared<Day4Hw2Subscriber>("lc_sub");
    rclcpp::spin(lc_listener);

    rclcpp::shutdown();

    return 0;
}