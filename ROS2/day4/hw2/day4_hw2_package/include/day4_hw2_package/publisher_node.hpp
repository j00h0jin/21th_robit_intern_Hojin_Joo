#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <utility>

#include "lifecycle_msgs/msg/transition.hpp"
#include "rclcpp/publisher.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "std_msgs/msg/string.hpp"

/**
 * 라이프사이클 탤커는 일반적인 "talker" 노드처럼 Node를 상속받는 대신 LifecycleNode를 상속받습니다.
 * 이를 통해 노드의 현재 상태에 따라 호출되는 일련의 콜백 함수들이 제공됩니다.
 * 모든 라이프사이클 노드에는 외부에서 제어하고 상태 변화를 유발할 수 있도록 연결된 서비스 세트가 있습니다.
 * 베타1 기준 사용 가능한 서비스:
 * - <node_name>__get_state
 * - <node_name>__change_state
 * - <node_name>__get_available_states
 * - <node_name>__get_available_transitions
 * 추가로, 상태 변화 알림을 위한 퍼블리셔가 생성됩니다:
 * - <node_name>__transition_event
 */

using namespace std::chrono_literals;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

class Day4Hw2Publisher : public rclcpp_lifecycle::LifecycleNode
{

  public:
    explicit Day4Hw2Publisher(const std::string &node_name, bool intra_process_comms = false)
        : rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms))
    {
    }

    void publish();

    CallbackReturn on_configure(const rclcpp_lifecycle::State &);
    CallbackReturn on_activate(const rclcpp_lifecycle::State &state);
    CallbackReturn on_deactivate(const rclcpp_lifecycle::State &state);
    CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
    CallbackReturn on_shutdown(const rclcpp_lifecycle::State &state);

  private:
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;

    std::shared_ptr<rclcpp::TimerBase> timer_;
};
