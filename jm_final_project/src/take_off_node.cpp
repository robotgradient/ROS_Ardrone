#include "ros/ros.h"
#include "std_msgs/Empty.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "take_off_node");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);

  ros::Publisher takeoff_pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1000);

  ROS_INFO("take off action");

  while (ros::ok())
  {
    std_msgs::Empty msg;
		
		takeoff_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
