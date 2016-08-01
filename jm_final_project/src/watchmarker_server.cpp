#include "ros/ros.h"
#include "jm_final_project/watch_marker.h"
#include "ar_pose/ARMarker.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>

double Tlast;
double Tmoment=0;

void arCallback(const ar_pose::ARMarker & msg){
	
	Tlast=ros::Time::now().toSec();
	//ROS_INFO("tiempo en el que ve %f",Tlast);
}

bool callservice(jm_final_project::watch_marker::Request  &req, jm_final_project::watch_marker::Response &res)

{
	double timelapse;
	Tmoment=ros::Time::now().toSec();
	timelapse=abs(Tmoment-Tlast);
	ROS_INFO("tiempolapse %f, Told = %f , Tnow = %f",timelapse,Tlast,Tmoment);
  if(timelapse>10){res.lost_marker=1;}	

	else
	{ res.lost_marker=0;}
	
	
  return true;
}

int main(int argc, char **argv)
{

ros::init(argc, argv, "Watchmarker_server");


ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("ar_pose_marker", 1, arCallback);
 ros::ServiceServer tservice = n.advertiseService("/watcher", callservice);
 
 ros::spin();

return 0;
}



