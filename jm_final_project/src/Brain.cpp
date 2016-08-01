#include <ros/ros.h>
#include <math.h>

#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Bool.h"

#include "std_msgs/Float64.h"

#include "geometry_msgs/Twist.h"

#include "std_srvs/Empty.h"
#include "jm_final_project/giro_toggle.h"
#include "jm_final_project/yawrange.h"
#include "jm_final_project/zrange.h"

#include "jm_final_project/watch_marker.h"


double T=0;
int step=0;
double Tgoal;
bool LOST_MARKER = 0;
bool YawOk =0;
bool ZOk=0;


void drone_land(const std_msgs::Empty & msg)
{
   step = 0;

}

void Begin(const std_msgs::Empty & msg)
{

	step=1;
	ROS_INFO("Step=1");
}

void next(const std_msgs::Empty & msg)
{

	step=step+1;

}




int main(int argc, char **argv)
{
  ros::init(argc, argv, "Brain");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);



	// service toggle camera
	ros::ServiceClient togglecameramain=n.serviceClient <std_srvs::Empty>("/ardrone/togglecam");	
	std_srvs::Empty togglecameramain_srv;
	
	//NEXT STEP
	
	ros::Subscriber nest=n.subscribe("/step_topic",1,next);

	

	// RQT INICIO
	ros::Subscriber init_sub=n.subscribe("/inicio_topic",1,Begin);

	// Giro/Toggle
	ros::Publisher GT=n.advertise<jm_final_project::giro_toggle>("/giro_toggle",1);

	// ARE U WATCHING THE MARKER??

	ros::ServiceClient Watch = n.serviceClient <jm_final_project::watch_marker> ("/watcher");

	// GIRAMOS SOBRE EL YAW!
	
	ros::ServiceClient End_Giro = n.serviceClient <jm_final_project::yawrange> ("/yaw_range");

	
	// ACERCAMIENTO A Z
	ros::ServiceClient Z = n.serviceClient <jm_final_project::zrange> ("/z_range");

	
  	//suscriber ready land
  	ros::Subscriber readyland_sub=n.subscribe("/ready_land",1,drone_land);

 	//publisher land
  	ros::Publisher land_pub = n.advertise<std_msgs::Empty>("/ardrone/land", 1);


  while(ros::ok()){


	if(step==1){ // SOBRE INICIAR STABILIZARSE EN EL MARKER 1
		
		togglecameramain.call(togglecameramain_srv);

		
		jm_final_project::giro_toggle M1;
		M1.toggle=1;
		M1.giro=0;
		
		GT.publish(M1);
		

		Tgoal = ros::Time::now().toSec() + 5;
		step=2;
		

	}//step=1

	else if(step==2){// SOBRE STABILIZARSE EN EL MARKER 1
		double T = ros::Time::now().toSec();
		if(T>Tgoal){
		
			jm_final_project::giro_toggle M2;
			M2.toggle=1;
			M2.giro=1;
		
			GT.publish(M2);
		
		
			step=3;
			ROS_INFO("han pasado 5 segundos");
		}

		else{

		jm_final_project::watch_marker srv;
		srv.request.time_moment = T;

		if (Watch.call(srv))
			{
				LOST_MARKER = srv.response.lost_marker;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 2!!");
			}
		if(LOST_MARKER==1){
			step= 0;
			std_msgs::Empty l;
     			land_pub.publish(l);
			ROS_INFO("Houston tenemos un problema. Aterrizaje Forzoso!!");
			
		}

		}
		

	}//step=2

	else if(step==3){ // SOBRE GIRAR MIRANDO EL MARKER 1
		
		jm_final_project::yawrange srvyaw;
		srvyaw.request.isinrange = 1;

		if (End_Giro.call(srvyaw))
			{
				YawOk = srvyaw.response.intorange;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 3!!");
			}

		if(YawOk==1){

			togglecameramain.call(togglecameramain_srv);

			
			jm_final_project::giro_toggle M3;
			M3.toggle=0;
			M3.giro=2;
		
			GT.publish(M3);
		
		
			step=4;
			ROS_INFO("Giro Concluido. Avance al Marker 2");
			ros::Duration(0.5).sleep();
			

		}
		else{

		jm_final_project::watch_marker srv3;
		srv3.request.time_moment = T;

		if (Watch.call(srv3))
			{
				LOST_MARKER = srv3.response.lost_marker;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 3.2!!");
			}
		if(LOST_MARKER==1){
			step= 0;
			std_msgs::Empty l;
     			land_pub.publish(l);
			ROS_INFO("Houston tenemos un problema. Aterrizaje Forzoso!!");
			
		}
	   }
	}//step=3

	else if(step==4){ // GOING TO MARKER 2
		jm_final_project::zrange srvz;
		srvz.request.isnear = 1;

		if (Z.call(srvz))
			{
				ZOk = srvz.response.isclose;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 4!!");
			}

		if(ZOk==1){

			
			jm_final_project::giro_toggle M4;
			M4.toggle=0;
			M4.giro=2;
			//posible change to 3
			GT.publish(M4);
		
			Tgoal = ros::Time::now().toSec() + 5;
			step=5;
			ROS_INFO("Near Marker 2. Now Stabilize!");
			

		}



		else{

		jm_final_project::watch_marker srv4;
		srv4.request.time_moment = T;

		if (Watch.call(srv4))
			{
				LOST_MARKER = srv4.response.lost_marker;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 4.2!!");
			}
		if(LOST_MARKER==1){
			step= 0;
			std_msgs::Empty l;
     			land_pub.publish(l);
			ROS_INFO("Houston tenemos un problema. Aterrizaje Forzoso!!");
			
		}
		}

	  }//step=4

	else if(step==5){
		double T = ros::Time::now().toSec();
		if(T>Tgoal){
		
			step= 0;
			jm_final_project::giro_toggle M5;
			M5.toggle=0;
			M5.giro=3;
			GT.publish(M5);
			std_msgs::Empty l;
     			land_pub.publish(l);
			ROS_INFO("Complete action sending Landing orders");
		}

		else{

		jm_final_project::watch_marker srv5;
		srv5.request.time_moment = T;

		if (Watch.call(srv5))
			{
				LOST_MARKER = srv5.response.lost_marker;
			}
		else
			{
			ROS_ERROR("Failed to call service in step 2!!");
			}
		if(LOST_MARKER==1){
			step= 0;
			std_msgs::Empty l;
     			land_pub.publish(l);
			ROS_INFO("Houston tenemos un problema. Aterrizaje Forzoso!!");
			
		}

		}
		

	}//step5



	 ros::spinOnce();
    	loop_rate.sleep();
   	}	//while
  }	//main
