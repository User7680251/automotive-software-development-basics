cmake_minimum_required(VERSION 3.0.2)
project(task3_package)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  geometry_msgs
  ugv_sdk
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable(scout_state_node src/scout_state_node.cpp)
add_executable(panel_node src/panel_node.cpp)
target_link_libraries(scout_state_node ${catkin_LIBRARIES})