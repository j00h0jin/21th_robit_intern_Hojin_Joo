from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    game_yaml_file_path = PathJoinSubstitution([
        FindPackageShare('cppqt_project_package'),
        'config',
        'game.yaml'
    ])

    return LaunchDescription([
        Node(
            package='cppqt_project_package',
            executable='player_node',
            name='player_node',
            parameters=[game_yaml_file_path],
            output='screen'
        ),
        Node(
            package='cppqt_project_package',
            executable='chaser_node',
            name='chaser_node',
            parameters=[game_yaml_file_path],
            output='screen'
        ),
        Node(
            package='cppqt_project_package',
            executable='game_gui_node',
            name='game_gui_node',
            parameters=[game_yaml_file_path],
            output='screen'
        )
    ])