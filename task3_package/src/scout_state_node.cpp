#include <ros/ros.h>
#include <iostream>
#include <unistd.h>
#include "ugv_sdk/scout/scout_base.hpp"
#include <geometry_msgs/Twist.h>

using namespace westonrobot;

void scout_state_node() {
  std::string device_name = "can0";
  std::string robot_subtype = "mini";
  int32_t baud_rate = 500000;

  bool is_scout_mini = (robot_subtype == "mini");

  std::unique_ptr<ScoutBase> scout;

  scout = std::make_unique<ScoutBase>(is_scout_mini);
  if (scout == nullptr) std::cout << "Failed to create robot object" << std::endl;

  scout->Connect(device_name);

  ros::NodeHandle nh;
  // 创建 ROS 发布器，发布 scout_vel 话题
  ros::Publisher vel_pub = nh.advertise<geometry_msgs::Twist>("/task3/scout_vel", 10);

  ros::Rate rate(50);  // 50 Hz
  int count = 0;
  while (ros::ok()) {
    // get robot state
    auto state = scout->GetScoutState();
    std::cout << "-------------------------------" << std::endl;
    // std::cout << "count: " << count << std::endl;
    std::cout << "control mode: " << static_cast<int>(state.control_mode)
              << " , vehicle state: " << static_cast<int>(state.base_state)
              << " , error code: " << std::hex << state.fault_code << std::dec
              << ", battery voltage: " << state.battery_voltage << std::endl;
    std::cout << "velocity (linear, angular): " << state.linear_velocity << ", "
              << state.angular_velocity << std::endl;
    std::cout << "motor state:"
              << "motor_num:" << state.motor_num << std::endl;

    auto actuator = state.actuator_states;
    for (int i = 0; i < 4; ++i) {
        printf("motor %d: current %f, rpm %d, driver temp %f, motor temp %f\n",
               actuator[i].motor_current,
               actuator[i].motor_rpm,
               actuator[i].driver_temperature,
               actuator[i].motor_temperature);
    }
    std::cout << "-------------------------------" << std::endl;

    // 创建一个 geometry_msgs::Twist 消息
    geometry_msgs::Twist vel_msg;
    vel_msg.linear.x = state.linear_velocity; // 设置线速度
    vel_msg.angular.z = state.angular_velocity; // 设置角速度

    // 发布消息到 scout_vel 话题
    vel_pub.publish(vel_msg);

    ros::spinOnce();
    rate.sleep();
    ++count;
  }
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "scout_state_node");
  ros::NodeHandle nh;
  ROS_INFO("\033[1;32m----> Scout_state_node Start\033[0m");

  scout_state_node();

  return 0;
}
