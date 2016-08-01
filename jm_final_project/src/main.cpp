#include <ros/ros.h>
#include <math.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>
#include "std_srvs/Empty.h"
float toggle=0;
float giro=0;
float takeoff=0;
float firsttime=0;
float takeenable=0;
float land_order=0;

void drone_takeoff(const std_msgs::Empty & msg)
{
   if (takeoff==1){takeenable=1;}
   takeoff=1;	
   //ros::Duration(2).sleep();
   ROS_INFO("taking off");

}

void drone_land(const std_msgs::Empty & msg)
{
   land_order=1;

}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "main");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);



// service toggle camera
ros::ServiceClient togglecameramain=n.serviceClient <std_srvs::Empty>("/ardrone/togglecam");
std_srvs::Empty togglecameramain_srv;

 // toggle  which indicates the camera front=0 bottom=1
 ros::Publisher toggle_pub = n.advertise<std_msgs::Float64>("/toggledetect", 1);

 // publisher of giro
 ros::Publisher giro_pub = n.advertise<std_msgs::Float64>("/giraco", 1);

  //suscriber al take off
   ros::Subscriber take_sub=n.subscribe("/ardrone/takeoff",1,drone_takeoff);
  //suscriber ready land
  ros::Subscriber readyland_sub=n.subscribe("/ready_land",1,drone_land);
 //publisher land
  ros::Publisher land_pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1);


  while(ros::ok()){
 
  if (takeoff==1 && firsttime==0 && takeenable==1)
   {	
	firsttime=1;
	//Once taking off-> togglecamera
	togglecameramain.call(togglecameramain_srv);
	
	toggle=1;
	std_msgs::Float64 msgcam;
	msgcam.data=1;
	toggle_pub.publish(msgcam);
	ROS_INFO("change of camera,toggle=%f",toggle);
	
	if(toggle==1)
	{
	//while stabilizing
	ros::Duration(5).sleep();
	ROS_INFO("han pasado 5 segundos");
	 //change again the camera
	togglecameramain.call(togglecameramain_srv);
	std_msgs::Float64 msgcam2;
	msgcam2.data=0;
	toggle_pub.publish(msgcam2);
	ROS_INFO("toggle change again,toggle=%f",toggle);
	//publish giro
	ros::Duration(0.3).sleep();
	std_msgs::Float64 msggiro;
	msggiro.data=1;
	giro=1;
	giro_pub.publish(msggiro);
	ROS_INFO("Main change de giro,giro=%f",giro);
	}
	
    }//if
    if(land_order==1){
	ros::Duration(5).sleep();
	std_msgs::Empty msg3;
     	land_pub.publish(msg3);
	ROS_INFO("landing and end of the experiment");
    }//second if
    ros::spinOnce();
    loop_rate.sleep();
   }	//while
  }	//main
