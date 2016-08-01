#include <ros/ros.h>
#include <math.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>
#include "sensor_msgs/Range.h"
#include "std_srvs/Empty.h"

float giro=0;
int counter=0;
int toggle=0;

void callbackgiro(const std_msgs::Float64 & msg)
{
	giro=msg.data;
	//ROS_INFO("giro=%f",giro);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "spin_controller");
  ros::NodeHandle n;

  ros::Rate loop_rate(10);
	
// service toggle camera
ros::ServiceClient togglecamera=n.serviceClient <std_srvs::Empty>("/ardrone/togglecam");
std_srvs::Empty togglecamera_srv;

 // toggle  which indicates the camera front=0 bottom=1
 ros::Publisher toggle_pub = n.advertise<std_msgs::Float64>("/toggledetect", 1);
  
  //el giro
  ros::Subscriber gir = n.subscribe("giraco", 1, callbackgiro);

  //land to the main
ros::Publisher readyland_pub = n.advertise<std_msgs::Empty>("/ready_land", 1);

  while(ros::ok()){
	if(giro==1){
	ros::Duration(0.5).sleep();
	std_msgs::Float64 msgcam;
	msgcam.data=1;
	toggle_pub.publish(msgcam);
	togglecamera.call(togglecamera_srv);	
	toggle=1;
	ROS_INFO("Bottom Camera from controller=%d",toggle);

	ros::Duration(0.6).sleep();
	togglecamera.call(togglecamera_srv);	
	toggle=0;
	std_msgs::Float64 msgcam2;
	msgcam.data=0;
	toggle_pub.publish(msgcam2);
	ROS_INFO("Front Camera from controller=%d",toggle);	
	}
	//when giro =2 nothing
	
	 ros::spinOnce();
    	loop_rate.sleep();
   }	//while
  }//int main
