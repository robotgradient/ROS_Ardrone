#include <ros/ros.h>
#include <math.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>
#include "sensor_msgs/Range.h"

float x_current,y_current,z_current,z_orient;

void ar_sightCallback(const ar_pose::ARMarker & msg)
{
     

 	ROS_INFO("MARKER DETECTED:x=%f y=%f z=%f", 				msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z);
	

}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "ar_sight");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);


  // velocidades y ar_pose
  ros::Subscriber sub = n.subscribe("ar_pose_marker", 1000, ar_sightCallback);
  

  while(ros::ok()){
	

	
	 ros::spinOnce();
    	loop_rate.sleep();
   }	//while
  }//int main
