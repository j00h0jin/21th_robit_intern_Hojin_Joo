import os

import cv2 as cv
import rclpy
import torch
from cv_bridge import CvBridge
from rclpy.node import Node
from rclpy.qos import QoSProfile
from sensor_msgs.msg import Image
from ultralytics import YOLO

current_dir = os.path.dirname(os.path.abspath(__file__))
model_path = os.path.join(current_dir, "..", "models", "yolo26n.pt")

model = YOLO(model_path)

class InferenceNode(Node):

    def __init__(self):
        super().__init__('inference_node')
        
        self.declare_parameter('topic_name', 'test') # Image
        self.topic_name = self.get_parameter('topic_name').value
        self.get_logger().info(f'topic_name: {self.topic_name}')
        
        qos_profile = QoSProfile(depth=10)
        self.image_sub = self.create_subscription(Image, self.topic_name, self.image_sub_callback, qos_profile)
        self.bridge = CvBridge()
        
    def image_sub_callback(self, msg):
        
        image = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')
        
        cv.imshow('Image', image)
        cv.waitKey(1)
        
    


def main(args=None):
    rclpy.init(args=args)
    node = InferenceNode()
    
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Keyboard Interrupt (SIGINT)')
    finally:
        if rclpy.ok():
            node.destroy_node()
            rclpy.shutdown()


if __name__ == '__main__':
    main()