#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>

#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#define OPENCV_WINDOW_COLOR "彩色图像窗口"
#define OPENCV_WINDOW_DEPTH "深度图像窗口"
#define WATERMARK_TEXT "X"

using namespace std;


class ImageConverter
{
    ros::NodeHandle nh;
    image_transport::ImageTransport it;
    image_transport::Subscriber image_sub_color;
    image_transport::Subscriber image_sub_depth;
    image_transport::Publisher image_pub_color;
    image_transport::Publisher image_pub_depth;

public:
    ImageConverter() : it(nh)
    {
        // 订阅输入的彩色图像和深度图像，并发布输出的图像
        image_sub_color = it.subscribe("/camera/color/image_raw", 1, &ImageConverter::imageCov, this);
        image_sub_depth = it.subscribe("/camera/depth/image_rect_raw", 1, &ImageConverter::depthCov, this);
        image_pub_color = it.advertise("/image_converter/output_video/color", 1);
        image_pub_depth = it.advertise("/image_converter/output_video/depth", 1);

        cv::namedWindow(OPENCV_WINDOW_COLOR);
        cv::namedWindow(OPENCV_WINDOW_DEPTH);
    }

    ~ImageConverter()
    {
        cv::destroyWindow(OPENCV_WINDOW_COLOR);
        cv::destroyWindow(OPENCV_WINDOW_DEPTH);
    }

    void imageCov(const sensor_msgs::ImageConstPtr &msg)
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        // 添加 WATERMARK
        cv::putText(cv_ptr->image, WATERMARK_TEXT, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(13, 17, 15), 2);

        // 更新 GUI 窗口
        cv::imshow(OPENCV_WINDOW_COLOR, cv_ptr->image);
        cv::waitKey(3);

        // 发布修改后的图像消息
        image_pub_color.publish(cv_ptr->toImageMsg());
    }

    void depthCov(const sensor_msgs::ImageConstPtr &msg)
    {
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);

        // 更新 GUI 窗口
        cv::imshow(OPENCV_WINDOW_DEPTH, cv_ptr->image);
        cv::waitKey(3);

        // 发布修改后的图像消息
        image_pub_depth.publish(cv_ptr->toImageMsg());
    }
};

int main(int argc, char **argv)
{
    ros::init(argc, argv, "camera_node");
    ROS_INFO("\033[1;32m----> Camera_node Start\033[0m");
    ImageConverter IC;
    ros::spin();
    return 0;
}
