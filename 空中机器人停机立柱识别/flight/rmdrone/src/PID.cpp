#include "PID.h"

void PID_Initialize(PID_Parameter *pid){
		pid->kp = 0;
		pid->ki = 0;
		pid->kd = 0;
		pid->errnow = 0;
		pid->errold1 = 0;
		pid->errold2 = 0;
		pid->out = 0;
}

void PD_Controller(PID_Parameter* pd){ // position
		float errP,errD;
		errP = pd->errnow;
		errD = pd->errnow - pd->errold1;
		pd->errold1 = pd->errnow;
		pd->out = pd->kp * errP + pd->kd * errD;
		if(pd->kp == 0 && pd->kd == 0){
			pd->out = 0;
		}
	
}

void PID_Controller(PID_Parameter* pid){ // Increment
		float errP,errI,errD;
		errP = pid->errnow - pid->errold1;
		errI = pid->errnow;
		errD = pid->errnow - 2*pid->errold1 + pid->errold2;
		
		pid->errold2 = pid->errold1;
		pid->errold1 = pid->errnow;
		
		pid->out = pid->kp * errP + pid->ki * errI + pid->kd * errD;
		if(pid->kp == 0 && pid->ki == 0 && pid->kd == 0){
			pid->out = 0;
		}

}
