#ifndef _POSCAL_H_
#define _POSCAL_H_

#include <math.h>
#include "stm32f4xx.h"
#include "usart1.h"

#define PI 3.1415926535
#define gravity 9.81



void CalculatePos(int16_t Pitch_2,int16_t Yaw_2,int16_t pitcherr,int16_t yawerr);



#endif