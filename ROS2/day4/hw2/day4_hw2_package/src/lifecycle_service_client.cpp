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
#include <future>
#include <memory>
#include <string>
#include <thread>

#include "lifecycle_msgs/msg/state.hpp"
#include "lifecycle_msgs/msg/transition.hpp"
#include "lifecycle_msgs/srv/change_state.hpp"
#include "lifecycle_msgs/srv/get_state.hpp"

#include "rclcpp/rclcpp.hpp"

using namespace std::chrono_literals;

// 제어할 대상 노드 이름
static constexpr char const *lifecycle_node = "lc_talker";

// 모든 라이프사이클 노드에는 다양한 서비스가 연결되어 있습니다.
// 관례에 따라 <node name>/<service name> 형식을 사용합니다.
// 이 데모에서는 get_state와 change_state를 사용하므로 두 서비스 토픽은 다음과 같습니다:
// lc_talker/get_state
// lc_talker/change_state
static constexpr char const *node_get_state_topic = "lc_talker/get_state";
static constexpr char const *node_change_state_topic = "lc_talker/change_state";

template <typename FutureT, typename WaitTimeT>
std::future_status wait_for_result(FutureT &future, WaitTimeT time_to_wait)
{
    auto end = std::chrono::steady_clock::now() + time_to_wait;
    std::chrono::milliseconds wait_period(100);
    std::future_status status = std::future_status::timeout;
    do
    {
        auto now = std::chrono::steady_clock::now();
        auto time_left = end - now;
        if (time_left <= std::chrono::seconds(0))
        {
            break;
        }
        status = future.wait_for((time_left < wait_period) ? time_left : wait_period);
    } while (rclcpp::ok() && status != std::future_status::ready);
    return status;
}

class LifecycleServiceClient : public rclcpp::Node
{
  public:
    explicit LifecycleServiceClient(const std::string &node_name) : Node(node_name)
    {
    }

    void init()
    {
        // 모든 라이프사이클 노드는 외부에서 노드와 상호작용할 수 있도록
        // 몇 가지 서비스를 자동으로 생성합니다.
        // 가장 중요한 두 가지는 GetState와 ChangeState입니다.
        client_get_state_ = this->create_client<lifecycle_msgs::srv::GetState>(node_get_state_topic);
        client_change_state_ = this->create_client<lifecycle_msgs::srv::ChangeState>(node_change_state_topic);
    }

    /// 노드의 현재 상태를 요청합니다.
    /**
     * 이 함수에서는 lc_talker 노드의 현재 상태를 묻는 서비스 요청을 보냅니다.
     * 주어진 time_out 내에 응답이 오면 노드의 현재 상태를 반환하고,
     * 그렇지 않으면 알 수 없는 상태(unknown state)를 반환합니다.
     * \param time_out 알 수 없는 상태를 반환하기 전까지 응답을 대기할 시간(초)
     */
    unsigned int get_state(std::chrono::seconds time_out = 3s)
    {
        auto request = std::make_shared<lifecycle_msgs::srv::GetState::Request>();

        if (!client_get_state_->wait_for_service(time_out))
        {
            RCLCPP_ERROR(get_logger(), "Service %s is not available.", client_get_state_->get_service_name());
            return lifecycle_msgs::msg::State::PRIMARY_STATE_UNKNOWN;
        }

        // lc_talker 노드의 현재 상태를 묻는 서비스 요청을 보냅니다.
        auto future_result = client_get_state_->async_send_request(request).future.share();

        // 노드로부터 응답이 올 때까지 대기합니다.
        // 요청 시간이 초과되면 알 수 없는 상태를 반환합니다.
        auto future_status = wait_for_result(future_result, time_out);

        if (future_status != std::future_status::ready)
        {
            RCLCPP_ERROR(get_logger(), "Server time out while getting current state for node %s", lifecycle_node);
            return lifecycle_msgs::msg::State::PRIMARY_STATE_UNKNOWN;
        }

        // 성공적인 응답을 받았습니다. 현재 상태를 출력합니다.
        if (future_result.get())
        {
            RCLCPP_INFO(get_logger(), "Node %s has current state %s.", lifecycle_node,
                        future_result.get()->current_state.label.c_str());
            return future_result.get()->current_state.id;
        }
        else
        {
            RCLCPP_ERROR(get_logger(), "Failed to get current state for node %s", lifecycle_node);
            return lifecycle_msgs::msg::State::PRIMARY_STATE_UNKNOWN;
        }
    }

    /// 상태 전환(Transition)을 호출합니다.
    /**
     * 서비스 요청을 보내어 "transition" ID에 해당하는 전환을 호출하고자 함을 알립니다.
     * 기본적으로 이러한 전환은 다음을 포함합니다:
     * - configure (설정)
     * - activate (활성화)
     * - cleanup (정리)
     * - shutdown (종료)
     * \param transition 호출할 전환을 지정하는 ID
     * \param time_out 알 수 없는 상태를 반환하기 전까지 응답을 대기할 시간(초)
     */
    bool change_state(std::uint8_t transition, std::chrono::seconds time_out = 3s)
    {
        auto request = std::make_shared<lifecycle_msgs::srv::ChangeState::Request>();
        request->transition.id = transition;

        if (!client_change_state_->wait_for_service(time_out))
        {
            RCLCPP_ERROR(get_logger(), "Service %s is not available.", client_change_state_->get_service_name());
            return false;
        }

        // 호출하고자 하는 전환(transition)과 함께 요청을 보냅니다.
        auto future_result = client_change_state_->async_send_request(request).future.share();

        // 노드로부터 응답이 올 때까지 대기합니다.
        // 요청 시간이 초과되면 알 수 없는 상태를 반환합니다.
        auto future_status = wait_for_result(future_result, time_out);

        if (future_status != std::future_status::ready)
        {
            RCLCPP_ERROR(get_logger(), "Server time out while getting current state for node %s", lifecycle_node);
            return false;
        }

        // 응답을 받았습니다. 성공 여부를 출력합니다.
        if (future_result.get()->success)
        {
            RCLCPP_INFO(get_logger(), "Transition %d successfully triggered.", static_cast<int>(transition));
            return true;
        }
        else
        {
            RCLCPP_WARN(get_logger(), "Failed to trigger transition %u", static_cast<unsigned int>(transition));
            return false;
        }
    }

  private:
    std::shared_ptr<rclcpp::Client<lifecycle_msgs::srv::GetState>> client_get_state_;
    std::shared_ptr<rclcpp::Client<lifecycle_msgs::srv::ChangeState>> client_change_state_;
};

/**
 * 노드의 기본 라이프사이클을 순차적으로 트리거하는 간단한 독립 스크립트입니다.
 * configure로 시작하여 activate, deactivate, activate, deactivate, cleanup,
 * 그리고 마지막으로 shutdown을 수행합니다.
 */
void callee_script(std::shared_ptr<LifecycleServiceClient> lc_client)
{
    rclcpp::WallRate time_between_state_changes(0.1); // 10초 대기

    using Transition = lifecycle_msgs::msg::Transition;

    // 설정 (configure)
    {
        if (!lc_client->change_state(Transition::TRANSITION_CONFIGURE))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 활성화 (activate)
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_ACTIVATE))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 비활성화 (deactivate)
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_DEACTIVATE))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 다시 활성화
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_ACTIVATE))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 그리고 다시 비활성화
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_DEACTIVATE))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 정리 (cleanup) 수행
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_CLEANUP))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }

    // 마지막으로 종료 (shutdown)
    // 참고: 어떤 종료 전환 ID를 호출할 것인지 정확히 지정해야 합니다.
    // 현재 unconfigured 상태에 있으므로 TRANSITION_UNCONFIGURED_SHUTDOWN을 호출해야 합니다.
    {
        time_between_state_changes.sleep();
        if (!rclcpp::ok())
        {
            return;
        }
        if (!lc_client->change_state(Transition::TRANSITION_UNCONFIGURED_SHUTDOWN))
        {
            return;
        }
        if (!lc_client->get_state())
        {
            return;
        }
    }
}

void wake_executor(std::shared_future<void> future, rclcpp::executors::SingleThreadedExecutor &exec)
{
    future.wait();
    // 스크립트가 완료되면 익스큐터를 깨웁니다.
    // https://github.com/ros2/rclcpp/issues/1916
    exec.cancel();
}

int main(int argc, char **argv)
{
    // stdout 버퍼를 강제로 플러시합니다.
    // 런치 파일 내에서 동시에 실행될 때도 모든 출력이 올바르게 동기화되도록 보장합니다.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    auto lc_client = std::make_shared<LifecycleServiceClient>("lc_client");
    lc_client->init();

    rclcpp::executors::SingleThreadedExecutor exe;
    exe.add_node(lc_client);

    std::shared_future<void> script = std::async(std::launch::async, callee_script, lc_client);

    auto wake_exec = std::async(std::launch::async, wake_executor, script, std::ref(exe));

    exe.spin_until_future_complete(script);

    rclcpp::shutdown();

    return 0;
}