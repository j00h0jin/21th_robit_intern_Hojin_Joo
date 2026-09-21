from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():

    return LaunchDescription([
        Node(
            package='vision_day2_hw1_package',
            executable='hw1_node',
            name='hw1_node',
        )
    ])