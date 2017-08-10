#include "Header.h"
#include "Control.h"

void permission_set(DJIDrone *drone,int &control_set_flag){
	if(control_set_flag == 0){
		control_set_flag = 1;
		drone->request_sdk_permission_control();
	}
}
