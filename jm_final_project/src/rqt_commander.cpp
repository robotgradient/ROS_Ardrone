#include <ros/ros.h>
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include <dynamic_reconfigure/server.h>
#include <jm_final_project/start_rqtConfig.h>
#include "std_srvs/Empty.h"

bool Start=0;
bool Step=0;
bool auto_mode=0;
bool stepchange=0;
bool startchange=0;
bool takeoffchange=0;
bool takingoff=0;

void callback(jm_final_project::start_rqtConfig &config,uint32_t level){
 /*ROS_INFO(
"reconfigure Request: %b %b",
	config.take_off,config.land);*/
	
	Start=config.Start;
	Step=config.Step;
	takingoff=config.take_off;
	if(Step==1){stepchange=1;}
	if(Start==1){startchange=1;}
	if(takingoff==1){takeoffchange=1;}
	auto_mode=config.automatic;
	
}
int main(int argc, char **argv)
{
  ros::init(argc, argv, "rqt_commander");
  ros::NodeHandle n;
  ros::Rate loop_rate(1);

  
 dynamic_reconfigure::Server<jm_final_project::start_rqtConfig>server;
  dynamic_reconfigure::Server<jm_final_project::start_rqtConfig>::CallbackType f;
  f=boost::bind(&callback,_1,_2);
  server.setCallback(f);


ros::Publisher auto_pub=n.advertise<std_msgs::Bool>("/automatic",1000);
ros::Publisher start_pub=n.advertise<std_msgs::Empty>("/inicio_topic",1000);
ros::Publisher step_pub=n.advertise<std_msgs::Empty>("/step_topic",1000);
 ros::Publisher takeoff_pub = n.advertise<std_msgs::Empty>("/ardrone/takeoff", 1000);


  while(ros::ok()){
	//automatic mode
	std_msgs::Bool msgauto;
	msgauto.data=auto_mode;
	auto_pub.publish(msgauto);

	if (takingoff==1 && takeoffchange==1){	
	std_msgs::Empty msg;
     	takeoff_pub.publish(msg);
	takeoffchange=0;
	}

	
	if (Start==1 && startchange==1){	
	std_msgs::Empty msg2;
     	start_pub.publish(msg2);
	startchange=0;
	}

	if (Step==1 && stepchange==1){	
	std_msgs::Empty msg3;
     	step_pub.publish(msg3);
	stepchange=0;
	}
	
	 ros::spinOnce();
    	loop_rate.sleep();
  }//while

  return 0;
}
