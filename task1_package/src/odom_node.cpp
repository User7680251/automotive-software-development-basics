#include <ros/ros.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include "scout_msgs/ScoutStatus.h"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>


ros::Publisher odom_pub;
ros::Time previous_time;
int init_flag = 0;
double current_x = 0.0;
double current_y = 0.0;
double current_theta = 0.0;

void publishOdometry()
{
    // 创建一个nav_msgs::Odometry消息
    nav_msgs::Odometry odom_msg;
    odom_msg.header.stamp = ros::Time::now();
    odom_msg.header.frame_id = "odom";
    odom_msg.child_frame_id = "base_link";

    // 设置里程计的位置信息
    odom_msg.pose.pose.position.x = current_x;
    odom_msg.pose.pose.position.y = current_y;
    odom_msg.pose.pose.position.z = 0.0;

    // 设置里程计的姿态信息（使用四元数表示）
    tf2::Quaternion quat;
    quat.setRPY(0, 0, current_theta);
    odom_msg.pose.pose.orientation = tf2::toMsg(quat);

    // 发布里程计消息
    odom_pub.publish(odom_msg);
}

void Odom(const scout_msgs::ScoutStatus::ConstPtr& msg)
{
    // 获取当前时间
    ros::Time current_time = ros::Time::now();
    if(init_flag == 0) previous_time = current_time, init_flag = 1;

    // 计算时间间隔
    double dt = (current_time - previous_time).toSec();

    // 获取线速度和角速度
    double linear_velocity = msg->linear_velocity;
    double angular_velocity = msg->angular_velocity;

    // 根据线速度、角速度和时间间隔计算位移和旋转量
    double delta_x = linear_velocity * dt;
    double delta_theta = angular_velocity * dt;

    // 更新姿态
    current_x += delta_x * cos(current_theta);
    current_y += delta_x * sin(current_theta);
    current_theta += delta_theta;

    // 更新上一个时间为当前时间，以便下一次计算
    previous_time = current_time;

    // 输出新的姿态
    ROS_INFO("Current Pose: x=%f, y=%f, theta=%f", current_x, current_y, current_theta);
    publishOdometry();
}

void callback(const scout_msgs::ScoutStatus::ConstPtr& msg)
{
    float lv = msg->linear_velocity;
    float av = msg->angular_velocity;
    ROS_INFO("linear_velocity: %f, angular_velocity: %f", lv, av);
    Odom(msg);
}

int main(int argc, char** argv)
{
    // 创建节点
    ros::init(argc, argv, "odom_node");
    ROS_INFO("\033[1;32m----> Odom_node Start\033[0m");
    ros::NodeHandle nh;

    // 订阅底盘 SDK 中的节点
    ros::Subscriber sub = nh.subscribe<scout_msgs::ScoutStatus>("/scout_status",1000, callback);

    odom_pub = nh.advertise<nav_msgs::Odometry>("/Odom", 1);

    // 节点持续
    ros::spin();
    return 0;
}
