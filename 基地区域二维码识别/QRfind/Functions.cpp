//
//  Functions.cpp
//  QRfind
//
//  Created by ding on 17/3/28.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "Functions.h"

const int QR_table[4][8] = {   //14347 41113 53276 39173
    {14347,45912,32619,31142,17849,13418,11941,8987},       //1 2 3
    {34513,27091,41113,27256,37943,32958,49998,43003},      //4   5
    {55492,42356,22060,40354,26014,55038,6861,53276},        //6 7 8
    {57317,29379,32001,60457,7766,39173,52118,35681}
};


void Final_center(const vector<Point> Predicted,Point & center,int bias,Point old_center){  //bias is a square number
    size_t vecsize = Predicted.size();
    int sort_x[vecsize],sort_y[vecsize];
    int x_mid,y_mid;
    int dis_x,dis_y;
    if(vecsize == 1){
        center = Predicted[0];
        return;
    }
    if(vecsize == 2){
        if(abs((Predicted[0].x - Predicted[1].x)*(Predicted[0].x - Predicted[1].x)) < bias && abs((Predicted[0].y - Predicted[1].y)*(Predicted[0].y - Predicted[1].y)) < bias){
            center.x = 0.5*(Predicted[0].x+Predicted[1].x);
            center.y = 0.5*(Predicted[0].y+Predicted[1].y);
            return;
        }else{
            center = old_center; // wait for change
            return;
        }
    }
    vector<Point> flited;
    for(size_t i=0;i<vecsize;i++){
        sort_x[i] = Predicted[i].x;
        sort_y[i] = Predicted[i].y;
    }
    sort(sort_x,sort_x+vecsize);
    sort(sort_y,sort_y+vecsize);
    
    x_mid = sort_x[vecsize/2];
    y_mid = sort_y[vecsize/2]; // already sorted
    
     for(size_t i=0;i<vecsize;i++){
         dis_x = Predicted[i].x - x_mid;
         dis_y = Predicted[i].y - y_mid;
         if(dis_x * dis_x < bias && dis_y * dis_y <bias){
             flited.push_back(Predicted[i]);
         }
     }
    size_t flited_size = flited.size();
    int x_result = 0,y_result = 0;
    if(flited_size == 1){
        center = flited[0];
    }else if(flited_size == 0){
        center = Point(x_mid,y_mid);
    }else{
        for(size_t i=0;i<flited_size;i++){
            x_result += flited[i].x;
            y_result += flited[i].y;
        }
        center = Point(x_result/flited_size,y_result/flited_size);
        
    }
    
    
    
    
}

void Predict_distance(const int num,const int bias,Point &dis){
    switch (num) {
        case 0:
            dis.x = bias;
            dis.y = bias;
            break;
        case 1:
            dis.x = 0;
            dis.y = bias;
            break;
        case 2:
            dis.x = -bias;
            dis.y = bias;
            break;
        case 3:
            dis.x = bias;
            dis.y = 0;
            break;
        case 4:
            dis.x = -bias;
            dis.y = 0;
            break;
        case 5:
            dis.x = bias;
            dis.y = -bias;
            break;
        case 6:
            dis.x = 0;
            dis.y = -bias;
            break;
        case 7:
            dis.x = -bias;
            dis.y = -bias;
            break;
    }
}

/*int Is_sameval(int group,int num){
    if(group == 0){
        if(num == 0 || num == 1) return 1;
    }
    if(group == 1){
        if(num == 2 || num == 4) return 1;
    }
    if(group == 2){
        if(num == 6 || num == 7) return 1;
    }
    if(group == 3){
        if(num == 3 || num == 5) return 1;
    }
    return 0;
}*/

void Center_predict(const int QRValue,const Point center,Point &out,size_t rectsize){
    int group,num;
    int bias = sqrt(rectsize)*25; // distance bias
    Point distance;
    for(group = 0;group < 4 ; group++){
        for(num = 0;num < 8;num++){
            if(QRValue == QR_table[group][num]){
                /*if(Is_sameval(group,num)) {
                    out = Point(0,0);
                    return;
                }*/
                Predict_distance(num, bias, distance);
                out.x = center.x + distance.x;
                out.y = center.y + distance.y;
                //return;
            }
        }
    }
}




void Classical_kalman(Point &bias,float err,Point &P_old,Point &P_new,Point P){
    Point2f newbias,K,Kg;
    P_old.x = P_new.x;
    P_old.y = P_new.y;
    newbias.x = P_old.x - P.x;
    newbias.y = P_old.y - P.y;
    K.x = fabs(2.0*newbias.x);
    K.y = fabs(2.0*newbias.y);
    Kg.x = sqrt((bias.x*bias.x+err*err)/((bias.x*bias.x+err*err)+K.x*K.x));
    Kg.y = sqrt((bias.y*bias.y+err*err)/((bias.y*bias.y+err*err)+K.y*K.y));
    bias.x=sqrt((1-Kg.x)*(bias.x*bias.x+err*err));
    bias.y=sqrt((1-Kg.y)*(bias.y*bias.y+err*err));
    P_new.x=P.x+Kg.x*(newbias.x);
    P_new.y=P.y+Kg.y*(newbias.y);
}

int IsQR(const Mat input,const int size_mat,int QRnum[4][4],Mat &gray,int& pixFlag){
    cvtColor(input, gray, CV_BGR2GRAY);
    threshold(gray, gray, 70, 255, THRESH_BINARY);
    Mat result;
    
    
    resize(gray, result, Size(6,6));
    //cout<<result<<endl;
    pixFlag = 0x0;
    for(int i=0;i<6;i++){
        if(getPixel(result, Point(0,i)) != 0){
            return 0;
        }
        if(getPixel(result, Point(5,i)) != 0){
            return 0;
        }
    }
    
    for (int ii = 0; ii < 4; ii++)
    {
        for (int j = 0; j < 4; j++)
        {
            int GP = 0;
            GP = getPixel(result, Point(ii + 1, j + 1));
            if (GP < 125)
            {
                pixFlag = pixFlag | (0 << (j * 4 + ii));
            }
            else
            {
                pixFlag = pixFlag | (1 << (j * 4 + ii));
            }
        }
    }
    
    return 1;
}

int getPixel(Mat img, Point pt) 	  //迭代器访问像素
{
    int gp = 0;
    
    uchar* data = img.ptr<uchar>(pt.y);
    gp = data[pt.x];
    
    return gp;
}



void QRProcess(Mat img,const vector<Point> contour,vector<Point> edge,const Point2f aff_val[4],Mat &out_img,int siz){
    Point2f edge_pt[4],edge_after[4];
    for(int i=0;i<4;i++){
        edge_pt[i] = Point2f(edge[i].x,edge[i].y);
    }
    sortCorner(edge_pt, edge_after);
    //Rect rect = boundingRect((Mat)contour);
    //Mat roi;
    //img(rect).copyTo(roi);
    Mat aff = getAffineTransform(edge_after, aff_val);
    warpAffine(img, out_img, aff, Size(siz,siz));
}


void Rect_finder(const vector<vector<Point> > contours,vector<vector<Point> > &out,vector<vector<Point> > &edge){
    vector<Point> approxRect;
    for(size_t i=0;i<contours.size();i++){
        approxPolyDP(contours[i], approxRect, arcLength(Mat(contours[i]),true)*0.05, true);
        if(approxRect.size() == 4){
            edge.push_back(approxRect);
            
            out.push_back(contours[i]);  // 1:1
        }
    }
}

Point get_contour_center(const vector<Point> contour){
    Point center;
    Moments mu;
    mu = moments(contour,false);
    center.x=mu.m10/mu.m00;
    center.y=mu.m01/mu.m00;
    return center;
}

void Draw_any_center(Mat &img,const vector<Point> contour,Point &center){
    Moments mu;
    mu = moments(contour,false);
    center.x=mu.m10/mu.m00;
    center.y=mu.m01/mu.m00;
    circle(img, center, 10, Scalar(255,255,255),-1);
}


/*---------------*/

void sortCorner(Point2f P1[4],Point2f N[4]){
    int X[4];
    int temp;
    Point2f Z1, Z2;
    for (int i = 0; i < 4; i++)
    {
        X[i] = P1[i].x;
    }
    for (int i = 0; i < 3; i++)
    {
        for (int ii = i + 1; ii < 4; ii++)
        {
            if (X[i] > X[ii])
            {
                temp = X[i];
                X[i] = X[ii];
                X[ii] = temp;
            }
        }
    }
    
    
    for (int i = 0; i < 4; i++)
    {
        if (X[0] == P1[i].x)
        {
            N[0] = P1[i];
            P1[i].x = 640;
            break;
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        if (X[2] == P1[i].x)
        {
            N[2] = P1[i];
            P1[i].x = 640;
            break;
        }
    }
    
    for (int i = 0; i < 4; i++)
    {
        if (X[1] == P1[i].x)
        {
            N[1] = P1[i];
            P1[i].x = 640;
            break;
        }
    }
    
    for (int i = 0; i < 4;i++)
    {
        if (X[3] == P1[i].x)
        {
            N[3] = P1[i];
            break;
        }
    }
    
    if (N[0].y > N[1].y)
    {
        Z1 = N[0];
        N[0] = N[1];
        N[1] = Z1;
    }
    if (N[2].y > N[3].y)
    {
        Z2 = N[2];
        N[2] = N[3];
        N[3] = Z2;
    }
    
}


/*---------------*/