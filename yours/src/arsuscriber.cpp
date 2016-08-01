#include <ros/ros.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include <dynamic_reconfigure/server.h>
#include <yours/dynparamsConfig.h>
#include "std_msgs/Float64.h"
float x_current,y_current,z_current,x_desired,y_desired,z_desired;
float errorz,errorx,errory,totalerror;
ros::Publisher errorpub;
void chatterCallback(const ar_pose::ARMarker & msg)
{
 ROS_INFO("I heard:x=%f y=%f z=%f", msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z);
x_current=msg.pose.pose.position.x;
y_current=msg.pose.pose.position.y;
z_current=msg.pose.pose.position.z;
	errorx=pow(x_desired-x_current,2);
	errory=pow(y_desired-y_current,2);
	errorz=pow(z_desired-z_current,2);
	totalerror=sqrt(errorx+errory+errorz);
	ROS_INFO("Error: %f",totalerror);

	
}
void callback(yours::dynparamsConfig &config,uint32_t level){
 ROS_INFO(
"reconfigure Request: %f %f %f",
	config.position_x,
	config.position_y,
	config.position_z);
x_desired=config.position_x;
y_desired=config.position_y;
z_desired=config.position_z;

	std_msgs::Float64 msg2;
    msg2.data = x_desired;
     errorpub.publish(msg2);
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "arsuscriber");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  ros::Subscriber sub = n.subscribe("ar_pose_marker", 1000, chatterCallback);
  dynamic_reconfigure::Server<yours::dynparamsConfig>server;
  dynamic_reconfigure::Server<yours::dynparamsConfig>::CallbackType f;
  f=boost::bind(&callback,_1,_2);
  server.setCallback(f);

  errorpub = n.advertise<std_msgs::Float64>("/arm/joint1_position_controller/command", 1000);
  while(ros::ok()){
	 ros::spinOnce();
    	loop_rate.sleep();
	
  }

  return 0;
}

