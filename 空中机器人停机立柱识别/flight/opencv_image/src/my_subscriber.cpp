#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
using namespace cv;
cv::Mat img;
VideoWriter writer("/home/ubuntu/catkin_ws/src/opencv_image/video/1.avi", CV_FOURCC('M', 'J', 'P', 'G'), 25.0, Size(640, 480));

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
		img = cv_bridge::toCvShare(msg, "bgr8")->image;
        //cv::imshow("view", img);
		writer<<img;
        // cv::WaitKey(30);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "image_listener");
    ros::NodeHandle nh;
    //cv::namedWindow("view");
    //cv::startWindowThread();
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
    ros::spin();
    //cv::destroyWindow("view");
}
