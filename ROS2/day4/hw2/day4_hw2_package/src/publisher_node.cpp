
#include "../include/day4_hw2_package/publisher_node.hpp"

void Day4Hw2Publisher::publish()
{
    static size_t count = 0;
    auto msg = std::make_unique<std_msgs::msg::String>();
    msg->data = "Lifecycle HelloWorld #" + std::to_string(++count);

    if (!pub_->is_activated())
    {
        RCLCPP_INFO(get_logger(), "currently inactive. Messages are not published.");
    }
    else
    {
        RCLCPP_INFO(get_logger(), "active. Publishing: [%s]", msg->data.c_str());
    }

    // 현재 상태와 무관하게 라이프사이클 퍼블리셔의 publish를 호출합니다.
    // 퍼블리셔가 활성(active) 상태인 경우에만 메시지 전송이 활성화되고 실제로 퍼블리시됩니다.

    // ROS2에서 lifecycle 노드의 콜백을 읽고 publish를 자동으로 열고 닫아준다.
    // active인 경우 publish됨, inactive인 경우 publish되지만 미들웨어 계층에서 버려진다고 한다.
    // ++count 부분은 계속해서 올라가면서 pub을 보내지만 버려지므로 sub에서 볼 수는 없음
    pub_->publish(std::move(msg));
}

// on_configure: Unconfigured 상태 -> inactive 상태로
// QoS 설정, 퍼블리셔 생성, 타이머 생성
CallbackReturn Day4Hw2Publisher::on_configure(const rclcpp_lifecycle::State &)
{
    // 이 콜백은 초기화 및 설정 목적으로 사용되도록 되어 있습니다.
    // 따라서 퍼블리셔와 타이머를 초기화하고 설정합니다.

    rclcpp::QoS qos_profile(10);                                      //  depth 10
    qos_profile.history(rclcpp::HistoryPolicy::KeepLast);             // history: keep last
    qos_profile.reliability(rclcpp::ReliabilityPolicy::Reliable);     // reliability: reliable
    qos_profile.durability(rclcpp::DurabilityPolicy::TransientLocal); //  durability: volatile

    // qos 설정은 위와 같이 history: keep last, depth: 10
    // relibility: reliable, durability: TransientLocal로 설정하였다.
    // 일반적인 통신을 위한 node라고 가정하고, 통신에서 자주 쓰는 QoS의 설정값을 참고한 뒤 설정을 일부 변경했다.

    // 사용 이유
    // history: 최근 데이터 유실을 방지하기 위해 몇개의 값을 저장하나, 너무 많은 메모리를 사용하지 않기 위해 10개로 설정
    // relibility: 일반적인 통신에서 데이터가 유실되는 것이 치명적일 수도 있기 때문에 유실되지 않도록 reliable로 설정
    // durability: 일반적으로 통신에서 durability에 사용되는 값은 volatile인데 TransientLocal로 설정을 해보았다.
    // publisher가 상태가 변경되는 경우에 유실되는 데이터를 저장하여 구독할 수 있도록 설정했다.

    pub_ = this->create_publisher<std_msgs::msg::String>("lc_topic", qos_profile);
    timer_ = this->create_wall_timer(1s, [this]() { return this->publish(); });

    RCLCPP_INFO(get_logger(), "on_configure");

    // 성공을 반환하여 다음 단계인 "inactive"로의 전환을 유발합니다.
    return CallbackReturn::SUCCESS;
}

// on_activate: inactive에서 active로
// Node 활성화, 데이터 전송 시작
CallbackReturn Day4Hw2Publisher::on_activate(const rclcpp_lifecycle::State &state)
{
    // 부모 클래스 메서드는 관리되는 엔티티(현재는 LifecyclePublisher)에 대해 자동으로 상태를 전환합니다.
    LifecycleNode::on_activate(state);

    RCLCPP_INFO(get_logger(), "on_activate");

    // 2초 동안 대기합니다. (활성화 단계 에뮬레이션)
    std::this_thread::sleep_for(2s);

    return CallbackReturn::SUCCESS;
}

// on_deactivate: active에서 inactive로
// Node 활성화 off, 데이터 전송 중지
CallbackReturn Day4Hw2Publisher::on_deactivate(const rclcpp_lifecycle::State &state)
{
    LifecycleNode::on_deactivate(state);

    RCLCPP_INFO(get_logger(), "on_deactivate");

    return CallbackReturn::SUCCESS;
}

// on_cleanup: inactived에서 unconfigured로
// 타이머와 퍼블리셔 해제, on_configure할 수 있는 상태로 돌아감
CallbackReturn Day4Hw2Publisher::on_cleanup(const rclcpp_lifecycle::State &)
{
    // 정리 단계에서 타이머와 퍼블리셔에 대한 공유 포인터를 해제합니다.
    timer_.reset();
    pub_.reset();

    RCLCPP_INFO(get_logger(), "on cleanup");

    return CallbackReturn::SUCCESS;
}

// on_shutdown: 아무런 어떠한 상태에서 Finalized로
// 타이머, 퍼블리셔 해제, 노드를 종료하도록 함
CallbackReturn Day4Hw2Publisher::on_shutdown(const rclcpp_lifecycle::State &state)
{
    // 종료 단계에서 타이머와 퍼블리셔에 대한 공유 포인터를 해제합니다.
    timer_.reset();
    pub_.reset();

    RCLCPP_INFO(get_logger(), "on shutdown", state.label().c_str());

    return CallbackReturn::SUCCESS;
}

int main(int argc, char *argv[])
{
    // stdout 버퍼를 강제로 플러시합니다.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    rclcpp::executors::SingleThreadedExecutor exe;

    std::shared_ptr<Day4Hw2Publisher> lc_node = std::make_shared<Day4Hw2Publisher>("lc_pub");

    exe.add_node(lc_node->get_node_base_interface());

    exe.spin();

    rclcpp::shutdown();

    return 0;
}