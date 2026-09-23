from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    hw1_yaml_file_path = PathJoinSubstitution([
        FindPackageShare('vision_day4_hw1_package'),
        'config',
        'hw1.yaml'
    ])

    return LaunchDescription([
        Node(
            package='vision_day4_hw1_package',
            executable='camera_node',
            name='camera_node',
            parameters=[hw1_yaml_file_path]
        ),
        Node(
            package='vision_day4_hw1_py_package',
            executable='inference_node',
            name='inference_node',
            parameters=[hw1_yaml_file_path]
        )
    ])