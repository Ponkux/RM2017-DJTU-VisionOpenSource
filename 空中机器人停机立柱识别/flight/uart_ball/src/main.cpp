#define MODE_P -8000
#define MODE_A 0
#define MODE_F 8000
#include <ros/ros.h>
#include <dji_sdk/dji_sdk.h>
#include <dji_sdk/dji_drone.h>
#include "mode_selecter/Remote_mode.h"
#include <iostream>
#include "serial.h"

#define GEAR_UP -10000
#define GEAR_DOWN -4545

int pos = 0;
int laser = 0;
int gear,mode,gear_old;
int choice = 0;
void deal_servomode(const dji_sdk::RCChannels::ConstPtr &msg){
	gear = msg->gear;
	mode = msg->mode;
	/*if(mode == MODE_P){
		if(gear == GEAR_UP) pos = 0;
		if(gear == GEAR_DOWN) pos = 1;
	} */
	if(mode == MODE_P && gear_old != gear) pos = !pos;
	gear_old = gear;
} 

void deal_camera_mode(const mode_selecter::Remote_mode::ConstPtr &msg){
	choice = msg->mode; 
	if(choice != 0){
		laser = 0;
	}else{
		laser = 1;
	}
}

int main(int argc, char *argv[]){
	int serialfd = 0;
	InitSerial(serialfd);
	ros::init(argc, argv,"uart_distance");
    ros::NodeHandle nh;
	ros::Subscriber sub_rc = nh.subscribe("dji_sdk/rc_channels",1,deal_servomode); 
	ros::Subscriber sub_mode = nh.subscribe("Remote_mode",1,deal_camera_mode);
	ros::spinOnce();
	while(ros::ok()){
		ros::spinOnce();
		TransformData(serialfd,pos,laser);
		//printf("pos:%d\n",pos); 
		usleep(20000);
	}

	return 0;
}
