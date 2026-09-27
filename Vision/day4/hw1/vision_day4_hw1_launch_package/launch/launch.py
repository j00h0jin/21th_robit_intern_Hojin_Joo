from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    camera_yaml_file_path = PathJoinSubstitution([
        FindPackageShare('vision_day4_hw1_package'),
        'config',
        'camera.yaml'
    ]),
    inference_yaml_file_path = PathJoinSubstitution([
        FindPackageShare('vision_day4_hw1_py_package'),
        'config',
        'inference.yaml'
    ])

    return LaunchDescription([
        Node(
            package='vision_day4_hw1_package',
            executable='camera_node',
            name='camera_node',
            parameters=[camera_yaml_file_path]
        ),
        Node(
            package='vision_day4_hw1_py_package',
            executable='inference_node',
            name='inference_node',
            parameters=[inference_yaml_file_path]
        )
    ])