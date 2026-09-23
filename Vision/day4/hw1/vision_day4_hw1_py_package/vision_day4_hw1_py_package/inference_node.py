import rclpy
import torch
from ultralytics import YOLO

model = YOLO('yolo26n.pt')
