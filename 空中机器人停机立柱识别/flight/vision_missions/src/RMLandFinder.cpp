//
//  RMLandFinder.cpp 
//  RMLandMission
//
//  Created by ding on 17/5/24.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "RMLandFinder.h"

RMLandFinder::RMLandFinder():kalmanerr(7.0){
    kalmanbias     = Point(20,20);
    OldCenter      = Point(0,0);
    NewCenter      = Point(0,0);
    Center_Procout = Point(0,0);
}

RMLandFinder::~RMLandFinder(){
    
}

void RMLandFinder::VariableReset(){
    kalmanerr      = 15.0;
    kalmanbias     = Point(20,20);
    OldCenter      = Point(0,0);
    NewCenter      = Point(0,0);
    Center_Procout = Point(0,0);
}

void RMLandFinder::Process(const Mat input,const int colorinput){
    img = input;
    color = colorinput;
    Thres = Mat::zeros(img.rows, img.cols, CV_8UC1);
    ThresProcess();
}

float RMLandFinder::Point_eudis(Point P1,Point P2){
    return sqrtf((P1.x - P2.x) * (P1.x - P2.x) + (P1.y - P2.y) * (P1.y - P2.y));
}

void RMLandFinder::ThresProcess(){
    split(img, splited);
    Mat imgr,imgg,imgb;
    if(color == BLUE){
        threshold((splited[0] - splited[1]), Thres, 50, 255, THRESH_BINARY);
    }else{
        threshold((splited[2] - splited[1]), Thres, 50, 255, THRESH_BINARY);
    }
    Triangles.clear();
    contours.clear();
    InitialCenters.clear();
    Mat naka = Thres.clone();
    
    findContours(naka, contours, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);
    //drawContours(img, contours, -1, Scalar(255,0,255),2);
    size_t contoursize = contours.size();
    size_t bigsize = 0,smallsize = 0;
    
    Point istri;
    OldCenter = Center_Procout;
    for(size_t i=0;i<contoursize;i++){
        if(bigsize < contours[i].size()){
            bigsize = contours[i].size();
            TheBiggest = contours[i];
        }
        approxPolyDP(contours[i], approx, 5, true);
        if(approx.size() == 3){
            center_g(contours[i],istri);
            if(getPixel(Thres, istri) > 128){
                Triangles.push_back(contours[i]);
                TriangleDirection(approx);
                smallsize = contours[i].size();
            }
        }
    }
    cout<<"small:"<<smallsize<<endl;
    cout<<"big:"<<bigsize<<endl;
    
    drawContours(img, Triangles, -1, Scalar(0,0,0),2);
    Merge();
    
    Classical_kalman(kalmanbias, kalmanerr, OldCenter, Center_Procout, NewCenter);
    
    if(smallsize != 0){
        if((bigsize / smallsize) > 4 ){
            center_g(TheBiggest, Center_Procout);
        }
    }else if(bigsize > 100){
        center_g(TheBiggest, Center_Procout);
    }else if(smallsize == 0){
        Center_Procout = Point(0,0);
    }
    
}
void RMLandFinder::TriangleDirection(vector<Point> Triangle){
    int a,b,c;
    a = Point_eudis(Triangle[0],Triangle[1]);
    b = Point_eudis(Triangle[1],Triangle[2]);
    c = Point_eudis(Triangle[2],Triangle[0]);
    float Angle_0 = acos((a*a+c*c-b*b)/(2.0*a*c))*180.0/CV_PI;
    float Angle_1 = acos((a*a+b*b-c*c)/(2.0*a*b))*180.0/CV_PI;
    float Angle_2 = acos((b*b+c*c-a*a)/(2.0*b*c))*180.0/CV_PI;
    
    float MAXVAL = MAX(MAX(Angle_0,Angle_1),Angle_2);
    
    Point Right,OtherOne,OtherTwo;
    
    //if(fabs(MAXVAL - 90.0) > 20.0) return;
    
    if(MAXVAL == Angle_0){
        Right    = Triangle[0];
        OtherOne = Triangle[1];
        OtherTwo = Triangle[2];
    }else if(MAXVAL == Angle_1){
        Right    = Triangle[1];
        OtherOne = Triangle[0];
        OtherTwo = Triangle[2];
    }else{
        Right    = Triangle[2];
        OtherOne = Triangle[0];
        OtherTwo = Triangle[1];
    }
    
    
    Point magic;
    magic.x = ( OtherOne.x + OtherTwo.x ) / 2.0;
    magic.y = ( OtherOne.y + OtherTwo.y ) / 2.0;
    Point scale = Point((Right.x - magic.x) , (Right.y - magic.y));
    Point answer;
    answer.x = Right.x + scale.x * 10.0; // value wait to change
    answer.y = Right.y + scale.y * 10.0;
    InitialCenters.push_back(answer);
    //circle(img, answer, 10,Scalar(255,0,0),-1);
}

void RMLandFinder::Merge(){
    size_t center_numbers = InitialCenters.size();
    if(center_numbers == 0) {
        NewCenter = Point(0,0);
        return;
    }
    if(center_numbers == 1){
        NewCenter = InitialCenters[0];
        return;
    }
    
    NewCenter = InitialCenters[0];
    for(size_t i=1;i<center_numbers;i++){
        NewCenter.x = (NewCenter.x + InitialCenters[i].x) / 2;
        NewCenter.y = (NewCenter.y + InitialCenters[i].y) / 2;
    }
    //circle(img, NewCenter, 10, Scalar(255,0,255),-1);
}


void RMLandFinder::dataout(Mat &image,Mat &thresimg,Point &Center){
    image = img;
    thresimg = Thres;
    Center = Center_Procout;
}

void RMLandFinder::center_g(const vector<Point> contour,Point &center){
    Moments mu;
    mu = moments(contour,false);
    center.x=mu.m10/mu.m00;
    center.y=mu.m01/mu.m00;
}

int RMLandFinder::getPixel(Mat image, Point pt){
    int gp = 0;
    uchar* data = image.ptr<uchar>(pt.y);
    gp = data[pt.x];
    return gp;
}


void RMLandFinder::Classical_kalman(Point &bias,float err,Point &P_old,Point &P_new,Point P){
    Point2f newbias,K,Kg;
    P_old.x   = P_new.x;
    P_old.y   = P_new.y;
    newbias.x = P_old.x - P.x;
    newbias.y = P_old.y - P.y;
    K.x       = fabs(2.0*newbias.x);
    K.y       = fabs(2.0*newbias.y);
    Kg.x      = sqrt((bias.x*bias.x+err*err)/((bias.x*bias.x+err*err)+K.x*K.x));
    Kg.y      = sqrt((bias.y*bias.y+err*err)/((bias.y*bias.y+err*err)+K.y*K.y));
    bias.x    = sqrt((1-Kg.x)*(bias.x*bias.x+err*err));
    bias.y    = sqrt((1-Kg.y)*(bias.y*bias.y+err*err));
    P_new.x   = P.x+Kg.x*(newbias.x);
    P_new.y   = P.y+Kg.y*(newbias.y);
}
