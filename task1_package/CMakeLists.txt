cmake_minimum_required(VERSION 3.0.2)
project(task1_package)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs

  scout_msgs
  
  cv_bridge
  image_transport

  tf2
  tf2_geometry_msgs
  nav_msgs
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

find_package(PCL REQUIRED)
include_directories(include ${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})

find_package(OpenCV REQUIRED)
include_directories(${OpneCV_INCLUDE_DIRS})

add_executable(odom_node src/odom_node.cpp)
add_executable(lidar_node src/lidar_node.cpp)
add_executable(camera_node src/camera_node.cpp)

target_link_libraries(lidar_node ${catkin_LIBRARIES} ${PCL_LIBRARIES})
target_link_libraries(camera_node ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
target_link_libraries(odom_node ${catkin_LIBRARIES})
