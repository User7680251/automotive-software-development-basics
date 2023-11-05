cmake_minimum_required(VERSION 3.0.2)
project(task4_package)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  cv_bridge
  rospy
  sensor_msgs
  std_msgs
)

find_package(OpenCV REQUIRED)

catkin_package(
CATKIN_DEPENDS roscpp rospy
)

include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

catkin_install_python(PROGRAMS
src/01_letter_recognition_node.py
src/02_nav_goal_node.py
DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
