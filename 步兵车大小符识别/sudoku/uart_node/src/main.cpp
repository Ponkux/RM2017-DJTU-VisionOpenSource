#include <iostream>
#include <ros/ros.h>
#include "serial.h"
#include "sudoku/sudoku_position.h"
#include "uart_node/sudoku_mode.h"

int which = 0,pos = 0,mode = 0;
int serialfd = 0;
void deal_pos(const sudoku::sudoku_position::ConstPtr& msg){
	which = msg->which;
	pos = msg->pos;
}
int tic = 0;
unsigned char data[10];
char transform[255];
int main(int argc,char *argv[]){
	ros::init(argc, argv, "Remote_mode");
	ros::NodeHandle nh;
	ros::Subscriber sub_pos = nh.subscribe("sudoku_position",1,deal_pos);
	ros::Publisher pub_mode = nh.advertise<uart_node::sudoku_mode>("sudoku_mode",1);
	InitSerial(serialfd);
	ros::spinOnce();
	while(ros::ok()){
		ros::spinOnce();
		GetMode(serialfd,mode);
		uart_node::sudoku_mode pmode;
		pmode.mode = mode;
		pub_mode.publish(pmode);
		if(mode != 0){
			if(which <= 0 || which > 5) which = 0;
			if(pos <= 0 || pos > 9) pos = 0;
			TransformData(serialfd,which,pos);
			std::cout<<"which:"<<which<<" pos:"<<pos<<std::endl;
		}
	}
	close(serialfd);
	return 0;
}
