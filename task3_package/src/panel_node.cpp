#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>

ros::Publisher vel_pub;
ros::Publisher pose_pub;

double wheelbase = 0.5;  // 轮距，根据实际数值进行调整
double x = 0.0; // 初始 x 坐标
double y = 0.0; // 初始 y 坐标
double theta = 0.0;  // 初始角度
ros::Time last_time;  // 上一次的时间戳

void scoutVelCallback(const geometry_msgs::Twist::ConstPtr& msg) {
  // 获取小车的线速度和角速度
  double v = msg->linear.x;
  double w = msg->angular.z;

  // 获取当前时间
  ros::Time current_time = ros::Time::now();

  // 计算时间间隔
  double dt = (current_time - last_time).toSec();

  // 更新上一次的时间戳
  last_time = current_time;

  // 更新坐标和姿态
  theta += w * dt;
  x += v * std::cos(theta) * dt;
  y += v * std::sin(theta) * dt;

  // 发布坐标和姿态
  nav_msgs::Odometry pose_msg;
  pose_msg.header.stamp = current_time;
  pose_msg.header.frame_id = "map";  // 坐标系根据实际情况进行调整
  pose_msg.pose.pose.position.x = x;
  pose_msg.pose.pose.position.y = y;
  pose_msg.pose.pose.position.z = 0.0;
  pose_msg.pose.pose.orientation = tf::createQuaternionMsgFromYaw(theta);

  pose_pub.publish(pose_msg);
}

int main(int argc, char** argv) {
  ros::init(argc, argv, "panel_node");
  ros::NodeHandle nh;
  ROS_INFO("\033[1;32m----> Panel_node Start\033[0m");

  // 创建 ROS 发布器
  pose_pub = nh.advertise<nav_msgs::Odometry>("task3/car_pose", 10);

  // 创建 ROS 订阅器，接收小车的线速度和角速度
  ros::Subscriber scout_vel_sub = nh.subscribe("/task3/scout_vel", 10, scoutVelCallback);

  // 初始化上一次的时间戳
  last_time = ros::Time::now();

  ros::spin();

  return 0;
}