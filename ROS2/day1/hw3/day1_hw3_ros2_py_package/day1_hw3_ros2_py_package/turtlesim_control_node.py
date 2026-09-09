import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from turtlesim.srv import SetPen
import sys
import select
import tty
import termios
import threading

class TurtlesimControl(Node):

    def __init__(self):
        super().__init__('turtlesim_control')
        self.publisher_ = self.create_publisher(Twist, 'turtle1/cmd_vel', 10)
        
        self.control_timer = self.create_timer(1.0, self.control_loop_callback)
        
        self.i = 0
        self.current_mode = None  
        self.is_running = True

        self.keyboard_thread = threading.Thread(target=self.keyboard_listener_loop)
        self.keyboard_thread.daemon = True
        self.keyboard_thread.start()
        
        self.pen_cli = self.create_client(SetPen, '/turtle1/set_pen')

    def keyboard_listener_loop(self):
        # 스레드 시작 시 터미널 설정을 한 번만 저장하고 Raw 모드로 변경
        settings = termios.tcgetattr(sys.stdin.fileno())
        try:
            tty.setraw(sys.stdin.fileno())
            while self.is_running and rclpy.ok():
                # 0.1초 단위로 키보드 입력이 있는지 확인
                rlist, _, _ = select.select([sys.stdin], [], [], 0.1)
                if rlist:
                    key = sys.stdin.read(1)
                    
                    # 현재 도형을 그리고 있지 않을 때만 새 명령 받기
                    if self.current_mode is None and key in ['w', 'a', 's', 'd']:
                        self.current_mode = key
                        self.i = 0
        finally:
            # 스레드가 끝날 때 터미널 설정을 원래대로 복구
            termios.tcsetattr(sys.stdin.fileno(), termios.TCSADRAIN, settings)

    def control_loop_callback(self):
        if self.current_mode is None:
            return

        if self.current_mode == 'w':
            self.circle()
        elif self.current_mode == 'a':
            self.triangle()
        elif self.current_mode == 's':
            self.rectangle()
        elif self.current_mode == 'd':
            self.pentagon()
    
    
    def set_pen(self, r, g, b, width, off=0):
        if not self.pen_cli.wait_for_service(timeout_sec=0.1):
            return
            
        req = SetPen.Request()
        req.r = r
        req.g = g
        req.b = b
        req.width = width
        req.off = off
        
        self.pen_cli.call_async(req)

    def circle(self):
        if self.i == 0:
            self.set_pen(0, 255, 0, 4)
            
        msg = Twist()
        msg.linear.x = 2.0  # 전진 속도
        msg.angular.z = 2.0  # 회전 속도
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
        
        if self.i > 4:
            stop_msg = Twist()
            self.publisher_.publish(stop_msg)
            self.current_mode = None
            self.i = 0
        self.i += 1
        
    def rectangle(self):
        if self.i == 0:
            self.set_pen(255, 0, 0, 5)
        msg = Twist()
        if self.i % 2 == 0:
            msg.linear.x = 3.0
            msg.angular.z = 0.0
        else:
            msg.linear.x = 0.0
            msg.angular.z = 1.5708  # 90도 = 1.5708rad
            
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: linear.x={msg.linear.x}, angular.z={msg.angular.z}')
        
        if self.i >= 7:
            stop_msg = Twist()
            self.publisher_.publish(stop_msg)
            self.current_mode = None
            self.i = 0
        self.i += 1
        
    def triangle(self):
        if self.i == 0:
            self.set_pen(255, 0, 255, 6)
        msg = Twist()
        if self.i % 2 == 0:
            msg.linear.x = 3.0
            msg.angular.z = 0.0
        else:
            msg.linear.x = 0.0
            msg.angular.z = 2.094  # 120도
            
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: linear.x={msg.linear.x}, angular.z={msg.angular.z}')
        
        if self.i >= 5:
            stop_msg = Twist()
            self.publisher_.publish(stop_msg)
            self.current_mode = None
            self.i = 0
        self.i += 1
        
    def pentagon(self):
        if self.i == 0:
            self.set_pen(0, 0, 255, 7)
        msg = Twist()
        if self.i % 2 == 0:
            msg.linear.x = 3.0
            msg.angular.z = 0.0
        else:
            msg.linear.x = 0.0
            msg.angular.z = 1.2566  # 72도
            
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: linear.x={msg.linear.x}, angular.z={msg.angular.z}')
        
        if self.i >= 9:
            stop_msg = Twist()
            self.publisher_.publish(stop_msg)
            self.current_mode = None
            self.i = 0
        self.i += 1

def main(args=None):
    rclpy.init(args=args)
    node = TurtlesimControl()
    try:
        rclpy.spin(node)
    except SystemExit:
        pass
    finally:
        node.destroy_node()
        if rclpy.ok():
            rclpy.shutdown()


if __name__ == '__main__':
    main()