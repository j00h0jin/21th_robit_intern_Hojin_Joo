from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    camera_ui_yaml_file_path = PathJoinSubstitution([
        FindPackageShare('vision_day1_package'),
        'config',
        'camera_ui.yaml'
    ])

    return LaunchDescription([
        Node(
            package='vision_day1_package',
            executable='camera_node',
            name='camera_node',
            parameters=[camera_ui_yaml_file_path]
        ),
        Node(
            package='vision_day1_package',
            executable='main_window',
            name='ui_node',
            parameters=[camera_ui_yaml_file_path]
        )
    ])