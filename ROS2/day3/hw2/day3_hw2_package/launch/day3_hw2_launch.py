from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 패키지 내 config/test.yaml 경로를 상대 참조로 생성
    yaml_file_path = PathJoinSubstitution([
        FindPackageShare('day3_hw2_package'),
        'config',
        'temp.yaml'
    ])

    return LaunchDescription([
        Node(
            package='day3_hw2_package',
            executable='traffic_light_node',
            name='traffic_light_node',
            parameters=[yaml_file_path]
        ),
        Node(
            package='day3_hw2_package',
            executable='vehicle_node',
            name='vehicle_node',
            parameters=[yaml_file_path]
        ),
        Node(
            package='day3_hw2_package',
            executable='day3_hw2_package',
            name='day3_hw2_gui'
        )
    ])