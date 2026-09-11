// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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

using namespace std::chrono_literals;

/// rclcpp_lifecycle::LifecycleNode를 상속받는 LifecycleTalker
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
class LifecycleTalker : public rclcpp_lifecycle::LifecycleNode
{
  public:
    /// LifecycleTalker 생성자
    explicit LifecycleTalker(const std::string &node_name, bool intra_process_comms = false)
        : rclcpp_lifecycle::LifecycleNode(node_name, rclcpp::NodeOptions().use_intra_process_comms(intra_process_comms))
    {
    }

    /// 메시지를 퍼블리시하기 위한 월 타이머 콜백
    void publish()
    {
        static size_t count = 0;
        auto msg = std::make_unique<std_msgs::msg::String>();
        msg->data = "Lifecycle HelloWorld #" + std::to_string(++count);

        // 데모 목적으로 현재 상태를 출력합니다.
        if (!pub_->is_activated())
        {
            RCLCPP_INFO(get_logger(), "Lifecycle publisher is currently inactive. Messages are not published.");
        }
        else
        {
            RCLCPP_INFO(get_logger(), "Lifecycle publisher is active. Publishing: [%s]", msg->data.c_str());
        }

        // 현재 상태와 무관하게 라이프사이클 퍼블리셔의 publish를 호출합니다.
        // 퍼블리셔가 활성(active) 상태인 경우에만 메시지 전송이 활성화되고 실제로 퍼블리시됩니다.
        pub_->publish(std::move(msg));
    }

    /// 상태 설정을 위한 전환 콜백
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_configure(
        const rclcpp_lifecycle::State &)
    {
        // 이 콜백은 초기화 및 설정 목적으로 사용되도록 되어 있습니다.
        // 따라서 퍼블리셔와 타이머를 초기화하고 설정합니다.
        pub_ = this->create_publisher<std_msgs::msg::String>("lifecycle_chatter", 10);
        timer_ = this->create_wall_timer(1s, [this]() { return this->publish(); });

        RCLCPP_INFO(get_logger(), "on_configure() is called.");

        // 성공을 반환하여 다음 단계인 "inactive"로의 전환을 유발합니다.
        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    /// 상태 활성화를 위한 전환 콜백
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_activate(
        const rclcpp_lifecycle::State &state)
    {
        // 부모 클래스 메서드는 관리되는 엔티티(현재는 LifecyclePublisher)에 대해 자동으로 상태를 전환합니다.
        LifecycleNode::on_activate(state);

        RCLCPP_INFO(get_logger(), "on_activate() is called.");

        // 2초 동안 대기합니다. (활성화 단계 에뮬레이션)
        std::this_thread::sleep_for(2s);

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    /// 상태 비활성화를 위한 전환 콜백
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_deactivate(
        const rclcpp_lifecycle::State &state)
    {
        LifecycleNode::on_deactivate(state);

        RCLCPP_INFO(get_logger(), "on_deactivate() is called.");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    /// 정리를 위한 전환 콜백
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_cleanup(
        const rclcpp_lifecycle::State &)
    {
        // 정리 단계에서 타이머와 퍼블리셔에 대한 공유 포인터를 해제합니다.
        timer_.reset();
        pub_.reset();

        RCLCPP_INFO(get_logger(), "on cleanup is called.");

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

    /// 종료를 위한 전환 콜백
    rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn on_shutdown(
        const rclcpp_lifecycle::State &state)
    {
        // 종료 단계에서 타이머와 퍼블리셔에 대한 공유 포인터를 해제합니다.
        timer_.reset();
        pub_.reset();

        RCLCPP_INFO(get_logger(), "on shutdown is called from state %s.", state.label().c_str());

        return rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn::SUCCESS;
    }

  private:
    // 라이프사이클 퍼블리셔 인스턴스를 보관합니다.
    std::shared_ptr<rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>> pub_;

    // publish 함수를 주기적으로 트리거하는 타이머 인스턴스를 보관합니다.
    std::shared_ptr<rclcpp::TimerBase> timer_;
};

int main(int argc, char *argv[])
{
    // stdout 버퍼를 강제로 플러시합니다.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    rclcpp::executors::SingleThreadedExecutor exe;

    std::shared_ptr<LifecycleTalker> lc_node = std::make_shared<LifecycleTalker>("lc_talker");

    exe.add_node(lc_node->get_node_base_interface());

    exe.spin();

    rclcpp::shutdown();

    return 0;
}