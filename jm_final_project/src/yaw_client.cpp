#include "ros/ros.h"
#include "jm_final_project/watch_marker.h"
#include "jm_final_project/zrange.h"
#include "std_msgs/Bool.h"
#include <cstdlib>

 int main(int argc, char **argv)
{
ros::init(argc, argv, "yaw_client");


ros::NodeHandle n;

//service yaw is into range
//ros::ServiceClient yaw_client = n.serviceClient<jm_final_project::yawrange>("yaw_range");
//ros::ServiceClient yaw_client = n.serviceClient<jm_final_project::watch_marker>("/watcher");

ros::ServiceClient yaw_client = n.serviceClient<jm_final_project::zrange>("/z_range");

//CODIGO De como llamar al service
  //jm_final_project::yawrange srv;
  //srv.request.isinrange = 0;

	/*jm_final_project::watch_marker srv;
	double Tlast;
  ROS_INFO("T last, %f", Tlast);

  srv.request.time_moment = 30;*/

jm_final_project::zrange srv;
srv.request.isnear=0;

   

  if (yaw_client.call(srv)) {
    ROS_INFO("Is close: %i", srv.response.isclose);
  }
  else {
    ROS_ERROR("Failed to call service add_two_ints");
    return 1;
  }
  return 0;
}

