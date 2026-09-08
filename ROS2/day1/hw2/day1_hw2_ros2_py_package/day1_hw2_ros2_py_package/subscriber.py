import rclpy
from rclpy.node import Node
from rclpy.qos import QoSProfile
from std_msgs.msg import String
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from std_msgs.msg import Float32

class HelloworldSubscriber(Node):

  def __init__(self):
    super().__init__('py_subscriber')
    qos_profile = QoSProfile(depth=10)
    self.helloworld_subscriber = self.create_subscription(String, 'string', self.subscribe_topic_message, qos_profile)
    self.int_subscriber = self.create_subscription(Int32, 'int', self.subscribe_topic_message, qos_profile)
    self.bool_subscriber = self.create_subscription(Bool, 'bool', self.subscribe_topic_message, qos_profile)
    self.float_subscriber = self.create_subscription(Float32, 'float', self.subscribe_topic_message, qos_profile)
    
  def subscribe_topic_message(self, msg):
    self.get_logger().info(f'Reveived message: {msg.data}')


def main(args=None):
  rclpy.init(args=args)
  node = HelloworldSubscriber()
  try:
    rclpy.spin(node)
  except KeyboardInterrupt:
    node.get_logger().info('Keyboard Interrupt (SIGINT)')
  finally:
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
  main()