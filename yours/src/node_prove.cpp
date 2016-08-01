#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
 ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "node_prove");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  ros::Subscriber sub = n.subscribe("chatter", 1000, chatterCallback);
 
ros::spin();

  return 0;
}
