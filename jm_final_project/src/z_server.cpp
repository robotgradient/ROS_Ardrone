#include "ros/ros.h"
#include "jm_final_project/zrange.h"
#include "ar_pose/ARMarker.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>

float Wall_distance;

void arCallback(const ar_pose::ARMarker & msg){
	Wall_distance=msg.pose.pose.position.z;
}

bool zcallback(jm_final_project::zrange::Request  &req, jm_final_project::zrange::Response &res)

{
  if(Wall_distance<1.4){res.isclose=1;}	

	else
	{ res.isclose=0;}
	ROS_INFO("Z distance has a value of %f",Wall_distance);

  return true;
}

int main(int argc, char **argv)
{

ros::init(argc, argv, "z_server");


ros::NodeHandle n;
 ros::Subscriber sub = n.subscribe("ar_pose_marker", 1, arCallback);
 ros::ServiceServer service = n.advertiseService("z_range", zcallback);
 
 ros::spin();

return 0;
}



