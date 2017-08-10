//
//  main.cpp
//  QRfind
//
//  Created by ding on 17/3/28.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include <iostream>
#include "All_Header.h"
#include "Functions.h"
#include <algorithm>

#define HEIGHT 300
#define WIDTH 400


int main(int argc, char * argv[]) {
    
    Mat img,gray_img;
    Mat undistort_frame,initialimg;
    
    Mat binary,bright,threshold_img,contours_img;
    
    //Mat camera_matrix = Mat(3, 3, CV_32FC1);
    //Mat distortion_coefficients;
    
    //FileStorage file_storage("camera_data_800_600.xml", FileStorage::READ);
    //file_storage["Camera_Matrix"] >> camera_matrix;
    //file_storage["Distortion_Coefficients"] >> distortion_coefficients;
    //file_storage.release();
    
    VideoCapture capture;
    capture.open(1);
    
    
    if(!capture.isOpened()){
        cout<<"camera err"<<endl;
        return -1;
    }else{
        capture.set(CV_CAP_PROP_FRAME_HEIGHT, 600);
        capture.set(CV_CAP_PROP_FRAME_WIDTH, 800);
    }
    
    vector<vector<Point> > contours,contours_fliter,contours_after,edge;
    vector<vector<Point2f>> edge_sorted;
    vector<Point2f> edge_sort_naka;
    vector<Vec4i> hierarchy;
    
    size_t mid_contour_size;
    
    Point center;
    Point2f affpoint[4] = {Point(0,0),Point(0,96),Point(96,0),Point(96,96)};
    Mat AFF33,Aff_dst;
    Mat Aff_result = Mat::zeros(80,80, CV_8UC3);
    Mat roi;
    size_t contour_size = 0;
    
    int QRnum[4][4] = {0};
    
    int pixFlag = 0,QRValue = 0;
    
    Point QR_center,Predicted_center;
    vector<Point> Predicted;
    size_t rectsize;
    Point final_center,final_center_old,final_center_new;
    
    Point kalman_bias(50,50);
    float kalman_err = 50;
    
    while(1){
        capture>>img;
        
        binary = Mat::zeros(HEIGHT, WIDTH, CV_8U);
        contours_img = Mat::zeros(HEIGHT, WIDTH, CV_8U);
        
        undistort_frame = img.clone();
        
        //undistort(img, undistort_frame, camera_matrix, distortion_coefficients);
        resize(undistort_frame,initialimg , Size(400,300));
        
        
        imshow("initial", initialimg);
        
        cvtColor(initialimg, binary, CV_BGR2GRAY);
        blur(binary,binary,Size(3,3));
        
        threshold(binary, threshold_img, 100, 255, THRESH_BINARY);
        findContours(threshold_img, contours, hierarchy, CV_RETR_TREE,  CV_CHAIN_APPROX_SIMPLE);
        
        for(int i=0;i<contours.size();i++){
            mid_contour_size = contours[i].size();
            if(mid_contour_size>15 && mid_contour_size < 200){
                contours_after.push_back(contours[i]);
            }
        }
        
        Rect_finder(contours_after, contours_fliter,edge);  //contours_fliter : contours of rectangle ; 
                                                            //edge : vec<vec<point>>  nx(4x4) vector
        
        /*for(int i=0;i<edge.size();i++){
            for(int j=0;j<4;j++){
                vec2pt[j] = edge[i][j];
            }
            sortCorner(vec2pt,edge_naka);
            
            for(int j=0;j<4;j++){
                //cout<<edge_naka[j]<<" ";
                edge_sort_naka.push_back(edge_naka[j]);
                //circle(initialimg, edge[i][j], 5, Scalar(255,255,255),-1);
            }
            //cout<<endl;
            edge_sorted.push_back(edge_sort_naka);
            edge_sort_naka.clear(); //edge_sorted point interval
        }
        
        for(int i=0;i<edge_sorted.size();i++){
            Rect rect = boundingRect((Mat)contours_fliter[i]); // minimum outside rectangle
            roi = initialimg(rect);
            //imshow("roi_init",roi);
            //cout<<roi<<endl;
            for(int j=0;j<4;j++){
                edge_aff[j] = edge_sorted[i][j];
            }
            cout<<edge_aff[0]<<" "<<edge_aff[1]<<" "<<edge_aff[2]<<" "<<edge_aff[3]<<endl;
            
            //AFF33 = getPerspectiveTransform(edge_aff, affpoint);
            AFF33 = getAffineTransform(edge_aff, affpoint);
            warpAffine(initialimg, roi, AFF33, roi.size());
            
            //warpPerspective(roi, Aff_result, AFF33, Size(80,80));
            imshow("roi",roi);
            roi.release();
            //Aff_result.release();
            
        }*/
        
        contour_size = contours_fliter.size();
        if(contour_size > 0){
            for(size_t i=0;i<contour_size;i++){
                rectsize = contours_fliter[i].size();
                QRProcess(initialimg, contours_fliter[i], edge[i], affpoint, Aff_dst, 96); // can be divided by 12
                imshow("dst",Aff_dst);
                if(IsQR(Aff_dst,96,QRnum,gray_img,pixFlag)){  //pixflag
                    imshow("gray",gray_img);
                    QR_center = get_contour_center(contours_fliter[i]);
                    QRValue = pixFlag;
                    //cout<<i<<"th "<<"QR: "<<QRValue<<endl;
                    circle(initialimg, QR_center,10, Scalar(255,255,255),-1);
                    if ((QRValue == 57317) || (QRValue == 29379) || (QRValue == 32001) || (QRValue == 60457) || (QRValue == 7766) || (QRValue == 52118) || (QRValue == 35681) || (QRValue == 45912) || (QRValue == 32619) || (QRValue == 31142) || (QRValue == 17849) || (QRValue == 13418) || (QRValue == 11941) || (QRValue == 8987) || (QRValue == 34513) || (QRValue == 27091) || (QRValue == 37943) || (QRValue == 27256) || (QRValue == 32958) || (QRValue == 49998) || (QRValue == 43003) || (QRValue == 55492) || (QRValue == 42356) || (QRValue == 22060) || (QRValue == 40354) || (QRValue == 26014) || (QRValue == 55038) || (QRValue == 6861)||(QRValue == 14347)||(QRValue == 41113)||(QRValue == 53276)||(QRValue == 39173))
                    {
                        Center_predict(QRValue, QR_center, Predicted_center, rectsize);
                        if(Predicted_center != Point(0,0)){
                            circle(initialimg, Predicted_center, 5, Scalar(0,0,255));
                            cout<<i<<"th point:("<<Predicted_center.x<<","<<Predicted_center.y<<")"<<endl;
                            Predicted.push_back(Predicted_center);
                        }
                        
                    }
                }
            }
        }
        final_center_old = final_center;
        Final_center(Predicted, final_center, 50,final_center_old);
        Classical_kalman(kalman_bias, kalman_err, final_center_old, final_center_new, final_center);
        
        circle(initialimg, final_center_new, 7, Scalar(255,0,0),-1);
        
        cout<<"pass"<<endl;
        drawContours(contours_img, contours_fliter, -1, Scalar(255, 255, 255));
        
        /*for(int i=0;i<contours_fliter.size();i++){
            Draw_any_center(initialimg,contours_fliter[i],center);
           // cout<<"The center of the "<<i<<"th contour is ("<<center.x<<","<<center.y<<")"<<endl;
        }*/
        imshow("centers",initialimg);
        imshow("contours_img", contours_img);
        
        
        
        
        
        
        //edge_sort_naka.clear();
        Predicted.clear();
        contours.clear();
        contours_fliter.clear();
        contours_after.clear();
        edge.clear();
        //edge_sorted.clear();
        waitKey(30);
    
        
        
    }
   
   
    
    return 0;
}
