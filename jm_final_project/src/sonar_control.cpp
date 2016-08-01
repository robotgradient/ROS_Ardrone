#include <ros/ros.h>
#include <math.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>
#include "std_srvs/Empty.h"
#include "ardrone_autonomy/Navdata.h"


float ON=0;
float alt;
float velz;

ros::Publisher vel_pub;


void ONOK(const std_msgs::Empty & msg)
{
  ros::Duration(5).sleep();
  ON=1;
}

void sonar(const ardrone_autonomy::Navdata & msg)
{

  if (ON ==1){
  	 alt = msg.altd;
	ROS_INFO("SONAR=%f",alt);
	
	velz = -0.001*(alt-1000);

	geometry_msgs::Twist velmsg;
	
	velmsg.linear.z=velz;
	
	vel_pub.publish(velmsg);

   	if(alt>900){
	ON=0;
	
	geometry_msgs::Twist velmsg;
	
	velmsg.linear.z=0;
	
	vel_pub.publish(velmsg);
		
	}
   
   }

  

}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "main");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);


  //suscriber al take off
   ros::Subscriber take_sub=n.subscribe("/ardrone/takeoff",1,ONOK);

  //SONAR
   ros::Subscriber SON=n.subscribe("/ardrone/navdata",1,sonar);
 
  //Publish Velocities

  vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);


  while(ros::ok()){
 
 
    ros::spinOnce();
    loop_rate.sleep();
   }	//while
  }	//main
