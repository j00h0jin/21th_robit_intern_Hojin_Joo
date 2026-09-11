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

#include <functional>
#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition_event.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

/// 간단한 리스너 노드 역할을 하는 LifecycleListener 클래스
/**
 * 두 개의 토픽을 구독합니다.
 * - lifecycle_chatter: talker로부터 오는 데이터 토픽
 * - lc_talker__transition_event: lc_talker 노드의 상태 변화에 대한 알림을 퍼블리시하는 토픽
 */
class LifecycleListener : public rclcpp::Node
{
  public:
    explicit LifecycleListener(const std::string &node_name) : Node(node_name)
    {
        // lc_talker 노드로부터 오는 데이터 토픽
        sub_data_ = this->create_subscription<std_msgs::msg::String>(
            "lifecycle_chatter", 10,
            [this](std_msgs::msg::String::ConstSharedPtr msg) { return this->data_callback(msg); });

        // 알림 이벤트 토픽. 모든 상태 변화는 전환을 나타내는 시작 상태와 목표 상태를 가진 TransitionEvent로 여기에
        // 퍼블리시됩니다.
        sub_notification_ = this->create_subscription<lifecycle_msgs::msg::TransitionEvent>(
            "/lc_talker/transition_event", 10, [this](lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg) {
                return this->notification_callback(msg);
            });
    }

    void data_callback(std_msgs::msg::String::ConstSharedPtr msg)
    {
        RCLCPP_INFO(get_logger(), "data_callback: %s", msg->data.c_str());
    }

    void notification_callback(lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg)
    {
        RCLCPP_INFO(get_logger(), "notify callback: Transition from state %s to %s", msg->start_state.label.c_str(),
                    msg->goal_state.label.c_str());
    }

  private:
    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::String>> sub_data_;
    std::shared_ptr<rclcpp::Subscription<lifecycle_msgs::msg::TransitionEvent>> sub_notification_;
};

int main(int argc, char **argv)
{
    // stdout 버퍼를 강제로 플러시합니다.
    // 런치 파일 내에서 동시에 실행될 때도 모든 출력이 올바르게 동기화되도록 보장합니다.
    setvbuf(stdout, NULL, _IONBF, BUFSIZ);

    rclcpp::init(argc, argv);

    auto lc_listener = std::make_shared<LifecycleListener>("lc_listener");
    rclcpp::spin(lc_listener);

    rclcpp::shutdown();

    return 0;
}