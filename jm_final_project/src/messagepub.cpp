#include "ros/ros.h"
#include "std_msgs/String.h"
#include "jm_final_project/giro_toggle.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "simple_publisher");
  ros::NodeHandle n;
  ros::Rate loop_rate(10);

  ros::Publisher chatter_pub = n.advertise<jm_final_project::giro_toggle>("mensaje", 1);

  ROS_INFO("-- Starting simple publisher node");

  while (ros::ok())
  {
    jm_final_project::giro_toggle msg;
		msg.toggle = 1;
    msg.giro=3;
		chatter_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
