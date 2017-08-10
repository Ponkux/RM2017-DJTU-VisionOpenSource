//
//  ImageProcess.h
//  sudoku_vold
//
//  Created by ding on 17/6/14.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef __sudoku_vold__ImageProcess__
#define __sudoku_vold__ImageProcess__

#include "Header.h"
#include "TString.h"
using namespace std;
using namespace cv;
namespace sudokuold{
    
typedef struct{
    int thres_val;
    int thres_detector[9];
    int contour_min;
    int contour_max;
    int targetsize;
	double alpha;
	double beta;
	int debug_flag;
}Params;

class RMPoint{
public:
    RMPoint(){}
    ~RMPoint(){};
    void data(Point s){this->x = s.x;this->y = s.y;};
    bool operator < (const RMPoint &s)const{
        if(s.x - this->x > 50){
            if(s.y - this->y > 20){
                return true;
            }else if(abs(s.y - this->y)<10){
                return true;
            }else{
                return false;
            }
        }else{
            if(s.y - this->y > 20){
                return true;
            }
        }
        return false;
    }
public:
    int x,y;
};

class TarPos{
public:
    TarPos(Point tl,Mat data,int keysize,int csize,Rect target){
        this->TL = tl;
        this->data = data;
        this->keysize = keysize;
        this->target = target;
		this->csize = csize;
    };
    ~TarPos(){};
    bool operator < (const TarPos &s)const{
		if(s.TL.y - this->TL.y > 40){
			return true;
		}else if(s.TL.x - this->TL.x > 60){
			return true;
		}
        return false;
    }
public:
    Point TL;
    Mat data;
    int keysize;
	int csize;
    Rect target;
};


class ImageProcess{
public:
    ImageProcess(){}
    ~ImageProcess(){};
    void Process(const Mat input);
    void ParamFresh(Params Parameters);
    
private:
    void ThresholdProcess();
    void ContourProcess();
    Mat aff_process(const Mat input,Point2f aff_point[4]);
    int ResultSolver(int new_result,int n);
    
public:
    Mat init,Binary,Binary_nouse;
    vector<TarPos> result_sort;
    int result;
private:
	int old_count = 30;
	int old_result;
    Mat gray;
	int lostflag;
    vector<vector<Point> >contours;
    Point2f aff_val[4];
    int thres_detector[9],thres_val,contour_min,contour_max,targetsize;
	double alpha,beta;
	int debug_flag;
    Mat kernel = getStructuringElement(MORPH_ERODE, Size(3,3));
};
    
}

#endif /* defined(__sudoku_vold__ImageProcess__) */
