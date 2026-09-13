from launch import LaunchDescription
from launch_ros.actions import LifecycleNode, Node


def generate_launch_description():
    publisher_node = LifecycleNode(
        package='day4_hw2_package',
        executable='publisher_node',
        name='lc_pub',
        namespace='',
        output='screen'
    )

    subscriber_node = Node(
        package='day4_hw2_package',
        executable='subscriber_node',
        name='lc_sub',
        output='screen'
    )

    return LaunchDescription([
        publisher_node,
        subscriber_node,
    ])