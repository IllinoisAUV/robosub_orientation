#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

#define IMAGE_TOPIC "/camera/image_raw"

int main(int argc, char** argv)
{
    
    // Check if video source has been passed as a parameter
    if( argc != 2) {
        ROS_ERROR("No video file provided");
        return 1;
    }
    
    ros::init(argc, argv, "image_publisher");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher imgPub = it.advertise(IMAGE_TOPIC, 10);

    cv::VideoCapture cap(argv[1]);
    
    // Check if video device can be opened with the given index
    if(!cap.isOpened() ) {
        ROS_ERROR("No opened capture");
        return 1;
    }

    cv::Mat frame;
    sensor_msgs::ImagePtr msg;
   
    ros::Rate loop_rate(5);
    while (ros::ok()) {
        
        cap >> frame;
        
        // Check if grabbed frame is actually full with some content
        if(!frame.empty()) {
            msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
            imgPub.publish(msg);
	    frame.release();
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
}
