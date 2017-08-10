//
//  RMLandFinder.h 
//  RMLandMission
//
//  Created by ding on 17/5/24.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef __RMLandMission__RMLandFinder__
#define __RMLandMission__RMLandFinder__

#include <stdio.h>
#include "Header.h"

enum{
    BLUE = 0,
    RED  = 1
};

class RMLandFinder{
public:
    RMLandFinder();
    ~RMLandFinder();
    void VariableReset();
    void Process(const Mat input,const int colorinput);
    void dataout(Mat &image,Mat &thresimg,Point &Center);
private:
    void ThresProcess();
    void center_g(const vector<Point> contour,Point &center);
    void TriangleDirection(vector<Point> Triangle);
    float Point_eudis(Point P1,Point P2);
    void Merge();
    int getPixel(Mat image, Point pt);
    void Classical_kalman(Point &bias,float err,Point &P_old,Point &P_new,Point P);
private:
    Mat img,Thres;
    int color;
    float kalmanerr;
    Point kalmanbias,OldCenter,NewCenter;
    Point Center_Procout;
    vector<Mat> splited;
    vector<vector<Point> > contours,Triangles;
    vector<Point> TheBiggest;
    vector<Point> approx;
    vector<Point> InitialCenters;
};

#endif 
/* defined(__RMLandMission__RMLandFinder__) */
