#include <ros/ros.h>
#include "std_msgs/String.h"
#include <dynamic_reconfigure/server.h>
#include <intros_exercise7/joint1paramsConfig.h>
#include "std_msgs/Float64.h"

float angle_desired;



	

void callback(intros_exercise7::joint1paramsConfig &config,uint32_t level){
 ROS_INFO(
"reconfigure Request: %f",
	config.Joint_angle);
	
	angle_desired=config.Joint_angle;



}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "anglepublisher");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  
  dynamic_reconfigure::Server<intros_exercise7::joint1paramsConfig>server;
  dynamic_reconfigure::Server<intros_exercise7::joint1paramsConfig>::CallbackType f;
  f=boost::bind(&callback,_1,_2);
  server.setCallback(f);

  ros::Publisher anglepub = n.advertise<std_msgs::Float64>("/arm/joint1_position_controller/command", 1000);
  while(ros::ok()){


	std_msgs::Float64 msg2;
    	msg2.data = angle_desired;
     	anglepub.publish(msg2);

	 ros::spinOnce();
    	loop_rate.sleep();
  }

  return 0;
}
