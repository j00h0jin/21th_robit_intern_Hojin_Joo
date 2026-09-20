from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    return LaunchDescription([
        Node(
            package='vision_day1_hw2_package',
            executable='hw2_node',
            name='hw2_node',
        )
    ])