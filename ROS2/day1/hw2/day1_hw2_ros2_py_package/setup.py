from setuptools import find_packages, setup

package_name = 'day1_hw2_ros2_py_package'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='hojin',
    maintainer_email='hojin7710@daum.net',
    description='TODO: Package description',
    license='TODO: License declaration',
    extras_require={
        'test': [
            'pytest',
        ],
    },
    entry_points={
        'console_scripts': [
            'day1_hw2_py_pub_node = day1_hw2_ros2_py_package.publisher:main',
            'day1_hw2_py_sub_node = day1_hw2_ros2_py_package.subscriber:main'
        ],
    },
)
