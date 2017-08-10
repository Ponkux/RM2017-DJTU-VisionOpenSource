#include <ros/ros.h>
#include "Header.h"
#include "PID.h"
#include "Control.h"

#define MODE_P -8000
#define MODE_A 0
#define MODE_F 8000

#define CenterX400 202
#define CenterY400 80
 
#define KP_x 0.0020
#define KP_y 0.0022  //lander PI
#define KD_x 0.0010
#define KD_y 0.0010

#define VP_x 3
#define VI_x 0.05    //velocity PID
#define VD_x 5
#define VP_y 3
#define VI_y 0.05 
#define VD_y 5

#define MAXSPEED 0.15
 
int choice = 0;
int center_x = 0,center_y = 0;
int control_flag = 0;
double vx,vy,vz;
float HeightBias = 0.0;
PID_Parameter InitSpd_x,InitSpd_y,CalSpd_x,CalSpd_y;

float ch0 = 0.0,ch1 = 0.0;
int landwait = 0,landConfirm = 0;
int mode = 0;

void VelocityHandle(const dji_sdk::Velocity::ConstPtr &msg){
	vx = msg->vx;
	vy = msg->vy;
	//vz = msg->vz;
	//cout<<"x:"<<vx<<" y:"<<vy<<" z:"<<vz<<endl;
}

void ModeHandle(const mode_selecter::Remote_mode::ConstPtr &msg){
	choice = msg->mode; 
}

void HeroHandle(const vision_missions::rm_hero::ConstPtr &msg){
	center_x = msg->x;
	center_y = msg->y;
	cout<<"Hero: x:"<<center_x<<" y:"<<center_y<<endl;
	if(center_x == 0 && center_y == 0){
		ch0 = 0.0;
		ch1 = 0.0;
		return ;
	}
	InitSpd_x.kp = KP_x;                       // Inner PD
	InitSpd_x.kd = KD_x;
	InitSpd_y.kp = KP_y;
	InitSpd_y.kd = KD_y;
	InitSpd_x.errnow = (center_x - CenterX400);
	InitSpd_y.errnow = (CenterY400 - center_y);
	PD_Controller(&InitSpd_x);
	PD_Controller(&InitSpd_y);

	CalSpd_x.kp = VP_x;                        // Out PID
	CalSpd_x.ki = VI_x;
	CalSpd_x.kd = VD_x;
	CalSpd_y.kp = VP_y;
	CalSpd_y.ki = VI_y;
	CalSpd_y.kd = VD_y;

	CalSpd_x.errnow = InitSpd_x.out - vx;
	CalSpd_y.errnow = InitSpd_y.out - vy;

	PID_Controller(&CalSpd_x);
	PID_Controller(&CalSpd_y);
	/*
	if(CalSpd_x.out > MAXSPEED){           //Double 
		ch0 = MAXSPEED;
	}else if(CalSpd_x.out < -MAXSPEED){
		ch0 = -MAXSPEED;
	}else{
		ch0 = CalSpd_x.out;
	}

	if(CalSpd_y.out > MAXSPEED){
		ch1 = MAXSPEED;
	}else if(CalSpd_y.out < -MAXSPEED){
		ch1 = -MAXSPEED;
	}else{
		ch1 = CalSpd_y.out;
	}
	*/

	if(InitSpd_x.out > MAXSPEED){			//Single
		ch0 = MAXSPEED;
	}else if(InitSpd_x.out < -MAXSPEED){
		ch0 = -MAXSPEED;
	}else{
		ch0 = InitSpd_x.out;
	}

	if(InitSpd_y.out > MAXSPEED){
		ch1 = MAXSPEED;
	}else if(InitSpd_y.out < -MAXSPEED){
		ch1 = -MAXSPEED;
	}else{
		ch1 = InitSpd_y.out;
	}

}

void LandHandle(const vision_missions::rm_land::ConstPtr &msg){
	center_x = msg->x;
	center_y = msg->y;
	cout<<"Land: x:"<<center_x<<" y:"<<center_y<<endl;
	if(center_x == 0 && center_y == 0){
		ch0 = 0.0;
		ch1 = 0.0;
		return ;
	}
	InitSpd_x.kp = KP_x;
	InitSpd_x.kd = KD_x;
	InitSpd_y.kp = KP_y;
	InitSpd_y.kd = KD_y;
	InitSpd_x.errnow = (center_x - CenterX400);
	InitSpd_y.errnow = (CenterY400 - center_y);
	PD_Controller(&InitSpd_x);
	PD_Controller(&InitSpd_y);

	/*CalSpd_x.kp = VP_x;                        // Out PID
	CalSpd_x.ki = VI_x;
	CalSpd_x.kd = VD_x;
	CalSpd_y.kp = VP_y;
	CalSpd_y.ki = VI_y;
	CalSpd_y.kd = VD_y;

	CalSpd_x.errnow = InitSpd_x.out - vx;
	CalSpd_y.errnow = InitSpd_y.out - vy;

	if(CalSpd_x.out > MAXSPEED){           //Double 
		ch0 = MAXSPEED;
	}else if(CalSpd_x.out < -MAXSPEED){
		ch0 = -MAXSPEED;
	}else{
		ch0 = CalSpd_x.out;
	}

	if(CalSpd_y.out > MAXSPEED){
		ch1 = MAXSPEED;
	}else if(CalSpd_y.out < -MAXSPEED){
		ch1 = -MAXSPEED;
	}else{
		ch1 = CalSpd_y.out;
	}*/

	
	if(InitSpd_x.out > MAXSPEED){
		ch0 = MAXSPEED;
	}else if(InitSpd_x.out < -MAXSPEED){
		ch0 = -MAXSPEED;
	}else{
		ch0 = InitSpd_x.out;
	}

	if(InitSpd_y.out > MAXSPEED){
		ch1 = MAXSPEED;
	}else if(InitSpd_y.out < -MAXSPEED){
		ch1 = -MAXSPEED;
	}else{
		ch1 = InitSpd_y.out;
	}
}

void HeightHandle(const dji_sdk::RCChannels::ConstPtr& msg){
	HeightBias = (float)(msg->throttle/10000.0)*0.5;
	mode = msg->mode;
}

void ParametersClear(){
	ch0 = 0;
	ch1 = 0;
	HeightBias = 0;
	landwait = 0;
	landConfirm = 0;
	control_flag = 0;
	PID_Initialize(&InitSpd_x);
	PID_Initialize(&InitSpd_y);
	PID_Initialize(&CalSpd_x);
	PID_Initialize(&CalSpd_y);
}

int main(int argc,char *argv[]){
	ros::init(argc, argv, "rmdrone");
    ros::NodeHandle nh;
    DJIDrone* drone = new DJIDrone(nh);

	ros::Subscriber sub_velocity = nh.subscribe("dji_sdk/velocity",1,VelocityHandle);	
	ros::Subscriber sub_hero = nh.subscribe("RM_HeroCenter",1,HeroHandle); //
	ros::Subscriber sub_land = nh.subscribe("RM_LandCenter",1,LandHandle);
	ros::Subscriber sub_mode = nh.subscribe("Remote_mode",1,ModeHandle);
	ros::Subscriber sub_Controller = nh.subscribe("dji_sdk/rc_channels",1,HeightHandle);
	PID_Initialize(&InitSpd_x);
	PID_Initialize(&InitSpd_y);
	PID_Initialize(&CalSpd_x);
	PID_Initialize(&CalSpd_y);
	
	ros::spinOnce();
	while(ros::ok()){
		ros::spinOnce();
		if(choice == 1 && mode == MODE_F){
			permission_set(drone, control_flag);
			drone->velocity_control(0,ch1,ch0,HeightBias,0);
			usleep(20000);
		}else if((choice == 2 || choice == 3) && mode == MODE_F){
			permission_set(drone, control_flag);
			if(landConfirm == 0){
				drone->velocity_control(0,ch1,ch0,HeightBias,0);
				usleep(20000);
				if(abs(center_x - CenterX400) < 7 && abs(center_y - CenterY400) < 7){
					landwait++;
				}
				if(landwait > 15){
					landConfirm = 1;
				}
			}
			if(landConfirm == 1){
				if(HeightBias <= 0.1){
					drone->request_sdk_permission_control();
					usleep(20000);
					drone->landing();
					sleep(8);
					drone->release_sdk_permission_control();
					usleep(20000);
				}else{
					landwait = 0;
					landConfirm = 0;
				}
			}
		}else{
			if(control_flag == 1){
				control_flag = 0;
				drone->release_sdk_permission_control();
			}
			ParametersClear();
			usleep(20000);
		}
	}
	delete drone;

}
