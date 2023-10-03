roscore
sudo modprobe gs_usb
sudo ip link set can0 up type can bitrate 500000

roslaunch scout_base scout_mini_base.launch
roslaunch scout_bringup open_rslidar.launch
roslaunch realsense2_camera rs_camera.launch

catkin_make --only-pkg-with-deps task1_package
source devel/setup.bash

rosrun task1_package lidar_node
rosrun task1_package odom_node
rosrun task1_package camera_node
