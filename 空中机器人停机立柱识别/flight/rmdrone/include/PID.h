#ifndef _RMDRONE_PID_H_
#define _RMDRONE_PID_H_

#include <iostream>



typedef struct{
	float kp;
	float ki;
	float kd;
	
	float errnow;
	float errold1;
	float errold2;
	float out;
}PID_Parameter;

void PID_Initialize(PID_Parameter *pid);
void PD_Controller(PID_Parameter* pd);
void PID_Controller(PID_Parameter* pid);

#endif
