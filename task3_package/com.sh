catkin_make --only-pkg-with-deps task3_package
source devel/setup.bash
rosrun task3_package scout_state_node

rosrun task3_package scout_state_node
rosrun task3_package panel_node
rosrun task3_package scout_control_node
