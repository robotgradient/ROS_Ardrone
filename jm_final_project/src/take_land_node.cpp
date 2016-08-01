#include <ros/ros.h>
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include <dynamic_reconfigure/server.h>
#include <jm_final_project/take_landConfig.h>
#include "std_srvs/Empty.h"

bool takingoff=0;
bool landing=0;
bool togglebool=0;
bool setparam=0;
bool girovariable=0;
bool togglechange=0;
bool auto_mode=0;
bool stopupdategiro=0;

void requestcallback(const std_msgs::Empty & msg)
{
 togglebool=1;
}	

void callback(jm_final_project::take_landConfig &config,uint32_t level){

	
	takingoff=config.take_off;
	landing=config.land;
	togglebool=config.togglecam;
	girovariable=config.giroparam;
	auto_mode=config.automatic;
	
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "take_land_node");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  
 dynamic_reconfigure::Server<jm_final_project::take_landConfig>server;
  dynamic_reconfigure::Server<jm_final_project::take_landConfig>::CallbackType f;
  f=boost::bind(&callback,_1,_2);
  server.setCallback(f);

ros::ServiceClient togglecamera=n.serviceClient <std_srvs::Empty>("/ardrone/togglecam");
std_srvs::Empty togglecamera_srv;
// these 2 ones are for a manual control of the process
// toggle  which indicates the camera front=0 bottom=1
 ros::Publisher toggle_pub = n.advertise<std_msgs::Float64>("/toggledetect", 1);

ros::Publisher giro_pub = n.advertise<std_msgs::Float64>("/giraco", 1);
  ros::Publisher takeoff_pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1000);
  ros::Publisher land_pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1000);
ros::Publisher auto_pub=n.advertise<std_msgs::Bool>("/automatic",1000);

//toggle request
ros::Subscriber  toggle_req_sus= n.subscribe("/toggle_request",1,requestcallback);
  //Publisher de que el toogle esta a front


  while(ros::ok()){
	//automatic mode
	std_msgs::Bool msgauto;
	msgauto.data=auto_mode;
	auto_pub.publish(msgauto);

	
	if (takingoff==1){	
	std_msgs::Empty msg2;
     	takeoff_pub.publish(msg2);
	}

	if (landing==1){
	std_msgs::Empty msg3;
     	land_pub.publish(msg3);
	}

	//toggle detect update
	if (togglebool==1){
	
	togglecamera.call(togglecamera_srv);
	togglechange=!togglechange;
	ROS_INFO("I'M CHANGING THE TOGGLE");
	togglebool=0;
		if (togglechange==1){
			std_msgs::Float64 msgcam;
			msgcam.data=1;
			ROS_INFO("cam %f",msgcam.data);
			toggle_pub.publish(msgcam);}
		else{
		  std_msgs::Float64 msgcam;
		  msgcam.data=0;
		  toggle_pub.publish(msgcam);
		ROS_INFO("cam %f",msgcam.data);}
		}

	if (girovariable==true & stopupdategiro==0){
	std_msgs::Float64 msggiro;
	msggiro.data=1;
	giro_pub.publish(msggiro);
	stopupdategiro=true;
	}
	
	 ros::spinOnce();
    	loop_rate.sleep();
  }

  return 0;
}
