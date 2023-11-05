#include <iostream>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <string.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/visualization/pcl_visualizer.h>
#include <boost/thread/thread.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl/io/io.h>


using namespace std;
boost::shared_ptr<pcl::visualization::PCLVisualizer> vis(new pcl::visualization::PCLVisualizer("PCL_point_cloud"));
ros::Publisher pub;
typedef pcl::PointCloud<pcl::PointXYZ> PointCloud;

void callback(const sensor_msgs::PointCloud2ConstPtr& rs_point_cloud)
{
    pcl::PCLPointCloud2* cloud = new pcl::PCLPointCloud2;
    pcl::PCLPointCloud2ConstPtr cloudPtr(cloud);
    pcl_conversions::toPCL(*rs_point_cloud, *cloud);
    pub.publish (*cloud);
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud1;
    cloud1.reset (new pcl::PointCloud<pcl::PointXYZ>);
    pcl::fromROSMsg (*rs_point_cloud, *cloud1);
    ROS_INFO("pcl_running");

    vis->removeAllPointClouds();  // 移除当前所有点云
    vis->addPointCloud(cloud1, "realtime pcl");
    vis->updatePointCloud(cloud1, "realtime pcl");
    vis->spinOnce(0.001);
}

int main(int argc, char** argv)
{
    // 创建节点
    ros::init(argc, argv, "lidar_node");
    ROS_INFO("\033[1;32m----> Lidar_node Start\033[0m");
    ros::NodeHandle nh;

    // 订阅雷达 SDK 中的节点
    ros::Subscriber sub = nh.subscribe("/rslidar_points", 10, callback);
    pub = nh.advertise<pcl::PCLPointCloud2> ("output", 10);

    // 节点持续
    ros::spin();
    return 0;
}
