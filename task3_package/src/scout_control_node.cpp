#include <ros/ros.h>
#include <iostream>
#include <unistd.h>
#include "ugv_sdk/scout/scout_base.hpp"

using namespace westonrobot;

int scout_control_node()
{
    std::string device_name = "can0";
    std::string robot_subtype = "mini";
    int32_t baud_rate = 500000;

    bool is_scout_mini = (robot_subtype == "mini");

    std::unique_ptr<ScoutBase> scout;

    scout = std::make_unique<ScoutBase>(is_scout_mini);
    if (scout == nullptr) std::cout << "Failed to create robot object" << std::endl;

    scout->Connect(device_name);

    scout->EnableCommandedMode();
    size_t count = 100;
    for (size_t i = 0; i < count; i++)
    {
        std::cout << "Motor:0.2,0" << std::endl;
        scout->SetMotionCommand(0.2, 0.0); //设定线速度为0.2

        usleep(20000);//挂起两万微秒,20毫秒
    }

    return 0;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "scout_control_node");
  ros::NodeHandle nh;
  ROS_INFO("\033[1;32m----> Scout_control_node Start\033[0m");

  scout_control_node();

  return 0;
}
