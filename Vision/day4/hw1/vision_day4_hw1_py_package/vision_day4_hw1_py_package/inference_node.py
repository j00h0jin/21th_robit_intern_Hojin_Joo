import os
import threading
import time

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
        
        self.image = None
        self.lock = threading.Lock()
        
        self.declare_parameter('topic_name', 'test') # Image
        self.declare_parameter('class_index', [0])
        self.declare_parameter('class_name', ['test'])
        self.declare_parameter('Hz', 1)
        
        self.topic_name = self.get_parameter('topic_name').value
        index = self.get_parameter('class_index').value
        name = self.get_parameter('class_name').value
        self.class_dict = dict(zip(index, name)) # 딕셔너리로 변환 {idx: name}
        Hz = self.get_parameter('Hz').value
        
        qos_profile = QoSProfile(depth=1)
        self.image_sub = self.create_subscription(Image, self.topic_name, self.image_sub_callback, qos_profile)
        
        timer_sec = 1 / Hz
        self.timer = self.create_timer(timer_sec, self.timer_callback)
        
    def image_sub_callback(self, msg):
        self.start_time = time.time()
        with self.lock:
            self.image = CvBridge().imgmsg_to_cv2(msg, desired_encoding='bgr8')
        
    def timer_callback(self):
        with self.lock:
            if self.image is None:
                return
            copy_image = self.image.copy()
        
        w = copy_image.shape[1]
        h = copy_image.shape[0]
        r = min(640/w, 640/h) # 1.333..., 1
        
        resized_img = cv.resize(copy_image, (int(w*r), int(h*r)))
        
        padding_w = int((640 - resized_img.shape[1]) / 2)
        padding_h = int((640 - resized_img.shape[0]) / 2)
        padded_img = cv.copyMakeBorder(resized_img, padding_h, padding_h, padding_w, padding_w,
                        borderType=cv.BORDER_CONSTANT, # borderType: 패딩을 어떻게 채울지, constant: 지정된 색으로 채움
                        value=(0, 0, 0)
                        )
        rgb_img = cv.cvtColor(padded_img, cv.COLOR_BGR2RGB) # rgb로
        # 기존 h, w, c -> c, h, w, float32 바꾸기
        tensor_img = torch.from_numpy(rgb_img).permute(2, 0, 1).to(dtype=torch.float32)
        tensor_img = tensor_img / 255.0 # 0 ~ 1
        tensor_img = tensor_img.unsqueeze(0) # n(배치) 추가 (n+chw)
        
        results = model.predict(source=tensor_img, conf= 0.3, iou=0.4)
        
        for box in results[0].boxes:
            idx = int(box.cls)
            conf = float(box.conf)
            
            # if idx가 목록에 있으면
            if idx in self.class_dict:
                class_name = self.class_dict[idx]

                coords = box.xyxy[0].tolist()
                _x1, _y1, _x2, _y2 = map(int, coords)
                
                # x_original = (x_input − pad_left) / r
                x1 = int((_x1 - padding_w) / r)
                y1 = int((_y1 - padding_h) / r)
                x2 = int((_x2 - padding_w) / r)
                y2 = int((_y2 - padding_h) / r)
                
                # 640x640 박스 coordinate -> 원본 이미지에 맞게 좌표 변환 후 그리기
                cv.rectangle(copy_image, (x1, y1), (x2, y2), (0, 255, 0), 3)
                
                label_text = f"{class_name} | confidence: {conf:.2f}"
                cv.putText(copy_image, label_text, (x1, max(20, y1 - 10)), 
                    cv.FONT_HERSHEY_SIMPLEX, 0.6, (0, 255, 0), 2)
            #
        #
        
        cv.imshow('yolo', copy_image)
        cv.waitKey(1)
        latency = (time.time() - self.start_time)
        self.get_logger().info(f'\nlatency: {latency:.3f}sec')


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