from launch import LaunchDescription
from launch_ros.actions import LifecycleNode, Node


def generate_launch_description():
    return LaunchDescription([
        LifecycleNode(package='lifecycle', executable='lifecycle_talker',
                      name='lc_talker', namespace='', output='screen'),
        Node(package='lifecycle', executable='lifecycle_listener', output='screen'),
        Node(package='lifecycle', executable='lifecycle_service_client', output='screen')
    ])