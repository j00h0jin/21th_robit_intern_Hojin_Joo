from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 패키지 내 config/test.yaml 경로를 상대 참조로 생성
    yaml_file_path = PathJoinSubstitution([
        FindPackageShare('day3_hw1_package'),
        'config',
        'test.yaml'
    ])

    return LaunchDescription([
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim_node'
        ),
        Node(
            package='day3_hw1_package',
            executable='day3_hw1_package', # cmakelists에 나와있는 노드 이름
            name='turtlesim_control',
            parameters=[yaml_file_path] # 파라미터 파일 전달
        )
    ])