#include "PosCal.h"
#define PitchMidPosC 5137

const long double GunHeight = 345; // cm
const double abspos[10][2] = {{0,0},
								{-370,810},{0,810},{370,810},
								{-370,630},{0,630},{370,630},
								{-370,400},{0,400},{370,400}};//(x,y)
/*const long double abspos[10][2] = {{0,0},
								{-370,700},{0,700},{370,700},
								{-370,450},{0,450},{370,450},
								{-370,200},{0,200},{370,200}};//(x,y)*/
extern int16_t FinalPos[10][2]; // pitch yaw
long double GravityOffset[10]={0};
long double alpha[4],theta[4];
long double CarDistance = 0;
extern int shootconfirmflag;
void  CalculatePos(int16_t Pitch_2,int16_t Yaw_2,int16_t pitcherr,int16_t yawerr){ // back position
	shootconfirmflag = 0;
	/*----------------------angle calculation-------------------*/
	alpha[1] = (PitchMidPosC - Pitch_2)*PI/4096.0; // rad +
	CarDistance = (abspos[2][1] - GunHeight)/tan(alpha[1]);
	theta[2] = (YawMidPos - Yaw_2)*PI/4096.0;
	long double Yleft = tan(theta[2])*CarDistance - (abspos[2][0] - abspos[1][0]); // -
	long double Yright = tan(theta[2])*CarDistance + (abspos[3][0] - abspos[2][0]); // +
	theta[1] = atan(Yleft/CarDistance);
	theta[3] = atan(Yright/CarDistance);
	long double Pmid  = tan(alpha[1])*CarDistance - (abspos[2][1] - abspos[5][1]);
	long double Plast = tan(alpha[1])*CarDistance - (abspos[2][1] - abspos[8][1]);
	alpha[2] = atan(Pmid/CarDistance);   // +
	alpha[3] = atan(Plast/CarDistance);  // -
	//printf("1:%Lf,2:%Lf,3:%Lf\n",alpha[1],alpha[2],alpha[3]);
	/*----------------------angle calculation-------------------*/
	
	/*----------------absolute position calculation-------------*/
	int16_t Yaw_1 = -atan(Yleft/CarDistance)*4096.0/PI + YawMidPos;// 180.0/PI*8192/360
	int16_t Yaw_3 = -atan(Yright/CarDistance)*4096.0/PI + YawMidPos;rm
	/*----------------absolute position calculation-------------*/
	
	/*------------------Gravity offset calculation--------------*/
    GravityOffset[1] = CarDistance/(cos(alpha[1])*cos(theta[1]))/1900.0; // definitely > 0 | t/s
	GravityOffset[2] = CarDistance/(cos(alpha[1])*cos(theta[2]))/1900.0;
	GravityOffset[3] = CarDistance/(cos(alpha[1])*cos(theta[3]))/1900.0;
	GravityOffset[4] = CarDistance/(cos(alpha[2])*cos(theta[1]))/1900.0;
	GravityOffset[5] = CarDistance/(cos(alpha[2])*cos(theta[2]))/1900.0;
	GravityOffset[6] = CarDistance/(cos(alpha[2])*cos(theta[3]))/1900.0;
	GravityOffset[7] = CarDistance/(cos(alpha[3])*cos(theta[1]))/1900.0;
	GravityOffset[8] = CarDistance/(cos(alpha[3])*cos(theta[2]))/1900.0;
	GravityOffset[9] = CarDistance/(cos(alpha[3])*cos(theta[3]))/1900.0;
	
	
	for(int i=1;i<10;i++){
		//GravityOffset[i] = 0.5*gravity*(GravityOffset[i]*GravityOffset[i]); // height
		GravityOffset[i] = 0;
	}
	
	/*------------------Gravity offset calculation--------------*/
	//printf("perr: %d\n",pitcherr);
	/*------------------Pitch final result----------------------*/
	int16_t height_1 = tan(alpha[1])*CarDistance;// +
	int16_t height_2 = Pmid; // +
	int16_t height_3 = Plast;// -
	//err large back > front | err small front > back
	int16_t pluserrup = pitcherr;
	int16_t pluserror = pitcherr;
	int16_t pluserrdown = pitcherr;
	int16_t yawerr_cal = yawerr*1.1;
	/*if(abs(pitcherr)>20){
		pluserrup = pitcherr>0? pitcherr*0.7:pitcherr*1.1;
		
		if(pitcherr>30 && pitcherr < 40){
			pluserror = pitcherr*(-0.15);
			pluserrdown = pitcherr*(-0.35);
		}else if(pitcherr >=40){
			pluserror = 0;
			pluserrdown = pitcherr*(-0.1);
		}
		else{
			pluserrdown = pitcherr>0? pitcherr*(-0.7):pitcherr*(1.21);
			pluserror = pitcherr>0? pitcherr*(-0.2):pitcherr*1.35;
		}
		yawerr_cal = yawerr;
	}else{
		pluserrup = pitcherr>0? pitcherr*0.7:pitcherr*1.1;
		if(abs(pitcherr)>7){
			if(pitcherr<0){
				pluserrdown = -30;
			}else{
				pluserrdown = -25;
			}
			pluserror = -40;
		}else{
			if(pitcherr<0){
				pluserror = -25;
			}else{
				pluserror = -17;
			}
			pluserrdown = -20;
		}
		
		yawerr_cal = yawerr*1.1;
		pluserror += 10;
	}*/
	
	FinalPos[1][0] = -atan((height_1 + GravityOffset[1])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrup-25; // 180.0/PI*8192/360
	FinalPos[2][0] = -atan((height_1 + GravityOffset[2])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrup-25;
	FinalPos[3][0] = -atan((height_1 + GravityOffset[3])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrup-25; // -20
	FinalPos[4][0] = -atan((height_2 + GravityOffset[4])/CarDistance)*4096.0/PI+PitchMidPosC+pluserror-45;
	FinalPos[5][0] = -atan((height_2 + GravityOffset[5])/CarDistance)*4096.0/PI+PitchMidPosC+pluserror-45;
	FinalPos[6][0] = -atan((height_2 + GravityOffset[6])/CarDistance)*4096.0/PI+PitchMidPosC+pluserror-45;
	FinalPos[7][0] = -atan((height_3 + GravityOffset[7])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrdown-100;
	FinalPos[8][0] = -atan((height_3 + GravityOffset[8])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrdown-100;//-25
	FinalPos[9][0] = -atan((height_3 + GravityOffset[9])/CarDistance)*4096.0/PI+PitchMidPosC+pluserrdown-100;
	/*------------------Pitch final result----------------------*/
	
	/*--------------------Yaw final result----------------------*/
	FinalPos[1][1] = Yaw_1 + yawerr_cal;
	FinalPos[4][1] = Yaw_1 + yawerr_cal;
	FinalPos[7][1] = Yaw_1 + yawerr_cal;
	FinalPos[2][1] = Yaw_2 ;
	FinalPos[5][1] = Yaw_2 ;
	FinalPos[8][1] = Yaw_2 ;
	FinalPos[3][1] = Yaw_3 - yawerr_cal;
	FinalPos[6][1] = Yaw_3 - yawerr_cal;
	FinalPos[9][1] = Yaw_3 - yawerr_cal;
	/*--------------------Yaw final result----------------------*/
	/*printf("(%d,%d),(%d,%d),(%d,%d)\n(%d,%d),(%d,%d),(%d,%d)\n(%d,%d),(%d,%d),(%d,%d)\n",FinalPos[1][0],FinalPos[1][1],
			FinalPos[2][0],FinalPos[2][1],FinalPos[3][0],FinalPos[3][1],FinalPos[4][0],FinalPos[4][1],FinalPos[5][0],
			FinalPos[5][1],FinalPos[6][0],FinalPos[6][1],FinalPos[7][0],FinalPos[7][1],FinalPos[8][0],FinalPos[8][1],FinalPos[9][0],FinalPos[9][1]);*/
}