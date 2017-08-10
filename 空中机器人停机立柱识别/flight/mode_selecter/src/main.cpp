#include <iostream>
#include <dji_sdk/dji_sdk.h>
#include <ros/ros.h>
#include <stdlib.h>
#include "mode_selecter/Remote_mode.h"

#define GEAR_UP -10000
#define GEAR_DOWN -4545

#define MODE_P -8000
#define MODE_A 0
#define MODE_F 8000

using namespace std;

int gear,gear_old,change_counter,Mode,tick,Mode_sub;

void gear_val(const dji_sdk::RCChannels::ConstPtr& msg){
	Mode = msg->mode;
	gear_old = gear;
	gear = msg->gear;
	if(Mode != MODE_F){
		Mode_sub = 0;
		tick = 0;
		change_counter = 0;
		return;
	}

	if(Mode == MODE_F && gear_old != 0 && Mode_sub == 0){
		if(gear_old != gear){
			change_counter++;
			tick = 0;
		}else if(change_counter != 0){
			tick++;
		}
	}
	if(tick > 25){
		Mode_sub = change_counter;
		change_counter = 0;
		tick = 0;
	}

}

int main(int argc,char *argv[]){
	ros::init(argc, argv, "Remote_mode");
	ros::NodeHandle nh;
	ros::Publisher pub_remote_mode = nh.advertise<mode_selecter::Remote_mode>("Remote_mode",1);
	ros::Subscriber sub_rc = nh.subscribe("dji_sdk/rc_channels",1,gear_val); 
	ros::spinOnce();
	while(ros::ok){
		ros::spinOnce();
		mode_selecter::Remote_mode r_mode;
		r_mode.mode = Mode_sub;
		pub_remote_mode.publish(r_mode);
		//cout<<Mode_sub<<endl;
		usleep(20000);
	}




}
