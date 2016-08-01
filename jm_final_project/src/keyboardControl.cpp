#include "ros/ros.h"
#include "std_msgs/Empty.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Twist.h"
#include <stdio.h>                //read input from terminal
#include <termios.h>              //termios, TCSANOW, ECHO, ICANON
#include <sys/select.h> 
#include "std_msgs/Bool.h"          //select and struct timeval, STDIN_FILENO and fd_set

#define SPEED 1

float LOOP_FREQ = 30;


bool  automatic=false;

void autom(const std_msgs::Bool & msg)
{  
 	automatic=msg.data;

}



int main(int argc, char **argv) {

  ros::init(argc, argv, "keyboardControl");
  ros::NodeHandle n;
  ros::Rate loop_rate(LOOP_FREQ);
  
  // publishers.
  ros::Publisher controller = n.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  ros::Subscriber  aut= n.subscribe("automatic",1,autom);
  
  std_msgs::Empty empty_msg;
  geometry_msgs::Twist controlSignal;
  geometry_msgs::Twist zeroSignal;
  zeroSignal.linear.x = 0;
  zeroSignal.linear.y = 0;
  zeroSignal.linear.z = 0;
  zeroSignal.angular.x = 0;
  zeroSignal.angular.y = 0;
  zeroSignal.angular.z = 0;

  
  char a;
  int res;


  fd_set readfds; //set of file descriptors
  struct timeval timeout; //timeout for waiting for an input in STDIN with getchar
  timeout.tv_sec = 0; //seconds
  timeout.tv_usec = 0; //microseconds


 
  static struct termios oldt, newt;

  /*tcgetattr gets the parameters of the current terminal
  STDIN_FILENO will tell tcgetattr that it should write the settings
  of stdin to oldt*/
  tcgetattr( STDIN_FILENO, &oldt);
  /*now the settings will be copied*/
  newt = oldt;

  /*ICANON normally takes care that one line at a time will be processed
  that means it will return if it sees a "\n" or an EOF or an EOL*/
  newt.c_lflag &= ~(ICANON | ECHO);

  /*Those new settings will be set to STDIN
  TCSANOW tells tcsetattr to change attributes immediately. */
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);
  
  float time = 0;
  float accel = 10;
  float elapsedTime = 1/LOOP_FREQ;

  while (ros::ok()) {

   	
    controlSignal = zeroSignal;
    
    //set timeout:
    timeout.tv_sec = 0; //seconds
    timeout.tv_usec = 0; //microseconds
    //initialize set
    FD_ZERO(&readfds); //clear set
    FD_SET(STDIN_FILENO, &readfds); //add stdin to the set
  
    res = select(STDIN_FILENO+1, &readfds, NULL, NULL, &timeout);
     ROS_INFO("CAWEN!");
    if (res > 0) { //res > 0 there is something to read. res == 0 timeout expired
      a = getchar();
      ROS_INFO("VALOR = %c",a);	
      
      switch (a) {
        
        
        case 'w': //Pitch forward
        {

          if (automatic==1) {
            controlSignal.linear.x = 0.15;
	    ROS_INFO("YUHUUU=%i",automatic);
          }
          break;
        }
	
        
        case 's': //Pitch backward
        {

          if (automatic==1) {
            controlSignal.linear.x = -0.15;

          }
          break;
        }
        
        case 'd': //Roll right
        {

          if (automatic==1) {
            controlSignal.linear.y = -0.15;


          }
          break;
        }
        
        case 'a': //Roll left
        {

          if (automatic==1) {
            controlSignal.linear.y = 0.15;

          }
          break;
        }
        
	
        case 'q': //null velocities
        {
          if (automatic) {
            controlSignal.linear.y = 0;
		controlSignal.linear.x = 0;
          }
          break;
        }

        case 'l': //Yaw right
        {
          if (automatic==1) {
            controlSignal.angular.z = -SPEED;
          }
          break;
        }
        
        case 'j': //Yaw left
        {
          if (automatic==1) {
            controlSignal.angular.z = SPEED;
          }
          break;
        }
        
   
        
        default: //Nothing
        break;
      }
    }
    
    if (res == 0) {
	time = 0;
    }
    
    if (res == -1) {
      ROS_INFO("ERROR with select!");
    }
    
    if (automatic==1) {
      controller.publish(controlSignal);
    }
    
    ros::spinOnce();
    loop_rate.sleep();
  }
  
  /*restore the old settings*/
  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
  
  return 0;
}
