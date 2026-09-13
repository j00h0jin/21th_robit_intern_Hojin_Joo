from launch import LaunchDescription
from launch.actions import ExecuteProcess, TimerAction
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
    configure_action = TimerAction(
        period=5.0,
        actions=[
            ExecuteProcess(
                cmd=['ros2', 'lifecycle', 'set', '/lc_pub', 'configure'],
                output='screen'
            )
        ]
    )
    activate_action = TimerAction(
        period=10.0,
        actions=[
            ExecuteProcess(
                cmd=['ros2', 'lifecycle', 'set', '/lc_pub', 'activate'],
                output='screen'
            )
        ]
    )
    deactivate_action = TimerAction(
        period=20.0,
        actions=[
            ExecuteProcess(
                cmd=['ros2', 'lifecycle', 'set', '/lc_pub', 'deactivate'], 
                output='screen'
            )
        ]
    )
    cleanup_action = TimerAction(
            period=25.0,
            actions=[
                ExecuteProcess(
                    cmd=['ros2', 'lifecycle', 'set', '/lc_pub', 'cleanup'], 
                    output='screen'
                )
            ]
        )
    shutdown_action = TimerAction(
        period=30.0,
        actions=[
            ExecuteProcess(
                cmd=['ros2', 'lifecycle', 'set', '/lc_pub', 'shutdown'], 
                output='screen'
            )
        ]
    )

    return LaunchDescription([
        publisher_node,
        subscriber_node,
        configure_action,
        activate_action,
        deactivate_action,
        cleanup_action,
        shutdown_action
    ])