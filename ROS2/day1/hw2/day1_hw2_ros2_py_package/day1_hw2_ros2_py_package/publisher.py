# import 구문
# rclpy의 Node 클래스 사용 / 퍼블리셔의 QoS 설정을 위한 QoSProfile 클래스
# 메시지 타입은 std_msgs.msg 모듈의 String 메시지 인터페이스 사용
import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile
from std_msgs.msg import String

# 이 노드의 메인 클래스는 HelloworldPublisher이고 Node 클래스를 상속해 사용할 예정
class HelloworldPublisher(Node):

  # 클래스 생성자 정의
  # 부모 클래스(Node)의 생성자를 호출하고 노드 이름을 helloworld_publisher로 지정
  def __init__(self):
    super().__init__('helloworld_publisher')
    qos_profile = QoSProfile(depth=10)
    
    # Nod 클래스의 create_publisher 함수를 이용해 helloworld_publisher를 설정함
    # 매개변수로 토픽 메시지 타입은 String, 토픽 이름은 helloworld, QoS는 qos_prpfile
    self.helloworld_publisher = self.create_publisher(String, 'helloworld', qos_profile)
    self.timer = self.create_timer(1, self.publish_helloworld_msg)
    self.count = 0
    
  # 앞에서 지정한 publish_helloworld_msg 콜백함수
  # 실제 데이터는 msg.data에 저장
  def publish_helloworld_msg(self):
    msg = String()
    msg.data = 'Hello World: {0}'.format(self.count)
    self.helloworld_publisher.publish(msg)
    self.get_logger().info('Published message: {0}'.format(msg.data))
    self.count += 1


# rclpy.init을 이용해 초기화
def main(args=None):
  rclpy.init(args=args)
  
  # HelloworldPublisher 클래스를 node 변수로 생성
  node = HelloworldPublisher()
  try:
  
  	# rclpy.spin 함수를 이용하여 생성한 노드를 spin 시켜 지정된 콜백 함수가 실행될 수 있도록
    rclpy.spin(node)
  except KeyboardInterrupt:
    node.get_logger().info('Keyboard Interrupt (SIGINT)')
  finally:
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
  main()