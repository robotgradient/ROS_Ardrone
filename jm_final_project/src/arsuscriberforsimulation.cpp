#include <ros/ros.h>
#include <math.h>
#include "ar_pose/ARMarker.h"
#include "std_msgs/String.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include <tf/transform_listener.h>
#include "sensor_msgs/Range.h"

float toggle=0;
float sonarH;
float pitch2=10000;
float x_current,y_current,z_current,z_orient;
float velx,vely,velz,angz;
float giro=0;
float readyforland=0;
float landonetime=0;
ros::Publisher vel_pub;

void tog(const std_msgs::Float64 & msg)
{
  //toggle=msg.data;
  
	if (toggle==0){toggle=1;}
	else{toggle=0;}
ROS_INFO("toggle=%f",toggle);
}

void sonarfunct(const sensor_msgs::Range & msg)
{
sonarH = msg.range;

}

void girico(const std_msgs::Float64 & msg)
{
giro=msg.data;
ROS_INFO("giro=%f",giro);
}



void arCallback(const ar_pose::ARMarker & msg)
{
     

 	/*ROS_INFO("Position:x=%f y=%f z=%f", 				msg.pose.pose.position.x,msg.pose.pose.position.y,msg.pose.pose.position.z);*/
	x_current=msg.pose.pose.position.x;
	y_current=msg.pose.pose.position.y;
	z_current=msg.pose.pose.position.z;
        
       

	// Convert quaternion to RPY
	tf::Quaternion qt;
	double roll,pitch,yaw;
	tf::quaternionMsgToTF(msg.pose.pose.orientation,qt);
	tf::Matrix3x3(qt).getRPY(roll,pitch,yaw);

	//ROS_INFO("Orientation:roll=%lf,pitch=%lf,yaw=%lf",roll,pitch,yaw);

	if(giro!=0){
	z_orient=pitch;
	pitch2=pitch;
	//ROS_INFO("pitch=%lf",pitch);
	}
	


 if(toggle==1)

	{
	if(y_current>0){velx=-y_current*y_current;}
	else if(y_current<0){velx=y_current*y_current;}
	else{velx=0;}
	if(x_current>0){vely=-x_current*x_current;}
	else if(x_current<0){vely=x_current*x_current;}
	else{vely=0;}
	if(z_current>0.8){velz=-0.01*z_current*z_current;}
	else if(z_current<0.7){velz=0.01*z_current*z_current;}
	else{velz=0;}
	
	

	geometry_msgs::Twist velmsg;
	velmsg.linear.x=velx;
	velmsg.linear.y=vely;
	velmsg.linear.z=velz;
	
	vel_pub.publish(velmsg);
	pitch2=1000;
	
	ROS_INFO("STABILIZING velocities:vx=%f,vy=%f,vz=%f,position:x=%f,y=%f,z=%f ",velx,vely,velz,x_current, y_current,z_current);	
	}

	

else if(toggle==0 && giro==2)
	{
       

        /*if(z_orient>0){angz=0.01*z_orient*z_orient;}
	else if(z_orient<0){angz=-0.01*z_orient*z_orient;}
	else{angz=0;}

	if(x_current>0){vely=-x_current*x_current;}
	else if(x_current<0){vely=x_current*x_current;}
	else{vely=0;}

	if(z_current>1.2 ){velx=0.1;}*/

        if(z_orient>0){angz=-2*z_orient*z_orient;}
	else if(z_orient<0){angz=2*z_orient*z_orient;}
	else{angz=0;}

	if(sonarH>1){velz=-0.1*sonarH*sonarH;}
	else if(sonarH<0.7){velz=0.1*sonarH*sonarH;}
	else{velz=0;}


        if(x_current>0){vely=-0.02*x_current*x_current;}
	else if(x_current<0){vely=0.02*x_current*x_current;}
	else{vely=0;}

	if(z_current>1 ){velx=0.1;
	ROS_INFO("Towards marker2 distance marker 2:%f",z_current);}
	else if(z_current<0.7 ){velx=-0.1*z_current*z_current;}
	else{velx=0;
	readyforland=1;
	ROS_INFO("Stabilizing distance:%f,height:%f",z_current,sonarH);}

	
        geometry_msgs::Twist velmsg;
	velmsg.linear.x=velx;
	velmsg.linear.y=vely;
	velmsg.linear.z=velz;
	velmsg.angular.z=angz;
	vel_pub.publish(velmsg);
	
	
	
	}

}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "arsuscriberforsimulation");
  ros::NodeHandle n;
  ros::Rate loop_rate(30);
  
  // sonar_height
  ros::Subscriber  SON= n.subscribe("/sonar_height",1,sonarfunct);

  // velocidades y ar_pose
  ros::Subscriber sub = n.subscribe("ar_pose_marker", 1, arCallback);
  vel_pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

  // el toggle
  ros::Subscriber subs_tog = n.subscribe("/toggledetect", 1, tog);
  
  //el giro
  ros::Subscriber gir = n.subscribe("giraco", 1, girico);

  //land to the main
ros::Publisher readyland_pub = n.advertise<std_msgs::Empty>("/ready_land", 1);

  while(ros::ok()){
	

	if(giro==1)
		{
		if(sonarH>0.8){velz=-0.1*sonarH*sonarH;}
		else if(sonarH<0.7){velz=0.1*sonarH*sonarH;}
		else{velz=0;}
	 
		geometry_msgs::Twist velmsg;
		velmsg.linear.x=0;
		velmsg.linear.y=0;
		velmsg.linear.z=velz;
		velmsg.angular.z=0.1;
		vel_pub.publish(velmsg);

		
		ROS_INFO("TURNING GYROS = %f,pitchon=%f",giro,pitch2);
		
		if(pitch2>-0.1 && pitch2<0.1)
		
		 {giro = 2;
		  ROS_INFO("Out of Turning proccess");}

		}//if
	if(readyforland==1 &&  landonetime==0)
		{landonetime=1;
		std_msgs::Empty landmsg;
     		readyland_pub.publish(landmsg);
		}//if
	
	 ros::spinOnce();
    	loop_rate.sleep();
   }	//while
  }//int main
	

