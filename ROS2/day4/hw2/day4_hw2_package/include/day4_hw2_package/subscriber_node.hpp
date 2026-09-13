#include <functional>
#include <memory>
#include <string>

#include "lifecycle_msgs/msg/transition_event.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

/**
 * 두 개의 토픽을 구독합니다.
 * - lifecycle_chatter: talker로부터 오는 데이터 토픽
 * - lc_talker__transition_event: lc_talker 노드의 상태 변화에 대한 알림을 퍼블리시하는 토픽
 */
class Day4Hw2Subscriber : public rclcpp::Node
{
  public:
    explicit Day4Hw2Subscriber(const std::string &node_name);
    void data_callback(std_msgs::msg::String::ConstSharedPtr msg);

    void notification_callback(lifecycle_msgs::msg::TransitionEvent::ConstSharedPtr msg);

  private:
    std::shared_ptr<rclcpp::Subscription<std_msgs::msg::String>> sub_data_;
    std::shared_ptr<rclcpp::Subscription<lifecycle_msgs::msg::TransitionEvent>> sub_notification_;
};