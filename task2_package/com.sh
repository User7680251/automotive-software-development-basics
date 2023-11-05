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

rosrun camera_calibration cameracalibrator.py --size 8x6 --square 0.024 image:=/camera/color/image_raw camera:=/camera/color/

# # camera_info
# height: 480
# width: 640
# distortion_model: "plumb_bob"
# D: [0.0, 0.0, 0.0, 0.0, 0.0]
# K: [607.9851684570312, 0.0, 317.3381042480469, 0.0, 606.8839721679688, 251.8374481201172, 0.0, 0.0, 1.0]
# R: [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0]
# P: [607.9851684570312, 0.0, 317.3381042480469, 0.0, 0.0, 606.8839721679688, 251.8374481201172, 0.0, 0.0, 0.0, 1.0, 0.0]
# binning_x: 0
# binning_y: 0
# roi: 
#   x_offset: 0
#   y_offset: 0
#   height: 0
#   width: 0
#   do_rectify: False

# # Ros_Calibrating
# mono pinhole calibration...
# D = [0.14178098028323127, -0.3025450954008717, -0.0041465594109068755, -0.021464845032590493, 0.0]
# K = [621.3293184058681, 0.0, 281.42996865200917, 0.0, 617.2445226960389, 239.33961489195906, 0.0, 0.0, 1.0]
# R = [1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0]
# P = [625.6741333007812, 0.0, 269.41406195744094, 0.0, 0.0, 637.2227783203125, 237.73580545768345, 0.0, 0.0, 0.0, 1.0, 0.0]
# None
# # oST version 5.0 parameters


# [image]

# width
# 640

# height
# 480

# [narrow_stereo]

# camera matrix
# 621.329318 0.000000 281.429969
# 0.000000 617.244523 239.339615
# 0.000000 0.000000 1.000000

# distortion
# 0.141781 -0.302545 -0.004147 -0.021465 0.000000

# rectification
# 1.000000 0.000000 0.000000
# 0.000000 1.000000 0.000000
# 0.000000 0.000000 1.000000

# projection
# 625.674133 0.000000 269.414062 0.000000
# 0.000000 637.222778 237.735805 0.000000
# 0.000000 0.000000 1.000000 0.000000

