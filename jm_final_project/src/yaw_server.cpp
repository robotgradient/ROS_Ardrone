#include "ros/ros.h"
#include "jm_final_project/yawrange.h"
#include "ar_pose/ARMarker.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>

double roll,pitch,yaw;

void arCallback(const ar_pose::ARMarker & msg){
	tf::Quaternion qt;	
	tf::quaternionMsgToTF(msg.pose.pose.orientation,qt);
	tf::Matrix3x3(qt).getRPY(roll,pitch,yaw);
}

bool add(jm_final_project::yawrange::Request  &req, jm_final_project::yawrange::Response &res)

{
  if(yaw<1.57+0.2 && yaw>1.57-0.2){res.intorange=1;}	

	else
	{ res.intorange=0;}
	//ROS_INFO("The yaw has a value of %f",yaw);

  return true;
}

int main(int argc, char **argv)
{

ros::init(argc, argv, "yaw_server");


ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("ar_pose_marker", 1, arCallback);
 ros::ServiceServer service = n.advertiseService("yaw_range", add);
 
 ros::spin();

return 0;
}



