//
//  Functions.h
//  QRfind
//
//  Created by ding on 17/3/28.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef __QRfind__Functions__
#define __QRfind__Functions__

#include <stdio.h>
#include "All_Header.h"
#include <algorithm>

enum{
    BLACK = 0,
    GREY =1,
    WHITE =2,
    RED =3,
    ORANGE =5,
    YELLOW =6,
    GREAN =7,
    C_BLUE =8,
    BLUE =9,
    PURPLE =10
};



void Final_center(const vector<Point> Predicted,Point & center,int bias,Point old_center);

int Is_sameval(int group,int num);

void Predict_distance(const int num,const int bias,Point &dis);

void Center_predict(const int QRValue,const Point center,Point &out,size_t rectsize);

void Classical_kalman(Point &bias,float err,Point &P_old,Point &P_new,Point P);

Point get_contour_center(const vector<Point> contour);

int IsQR(const Mat input,const int size_mat,int QRnum[4][4],Mat &gray,int& pixFlag);

void Rect_finder(const vector<vector<Point> > contours,vector<vector<Point> > &out,vector<vector<Point> > &edge);

void Draw_any_center(Mat &img,const vector<Point> contour,Point &center);

void sortCorner(Point2f P1[4],Point2f N[4]);

void QRProcess(Mat img,const vector<Point> contour,vector<Point> edge,const Point2f aff_val[4],Mat &out_img,int siz);

int getPixel(Mat img, Point pt) ;



#endif /* defined(__QRfind__Functions__) */
