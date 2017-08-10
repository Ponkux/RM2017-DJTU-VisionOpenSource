//
//  ImageProcess.cpp
//  sudoku_vold
//
//  Created by ding on 17/6/14.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "sudoku_old/ImageProcess.h"

namespace sudokuold {
    
void ImageProcess::ParamFresh(Params Parameters){
    thres_val   = Parameters.thres_val;
    contour_min = Parameters.contour_min;
    contour_max = Parameters.contour_max;
    targetsize  = Parameters.targetsize;
	alpha       = Parameters.alpha;
	beta        = Parameters.beta;
    debug_flag  = Parameters.debug_flag;
    for(int i=0;i<9;i++){
        thres_detector[i] = Parameters.thres_detector[i];
    }
    aff_val[0] = Point2f(0,0);
    aff_val[1] = Point2f(targetsize,0);
    aff_val[2] = Point2f(0,targetsize);
    aff_val[3] = Point2f(targetsize,targetsize);
}

void ImageProcess::Process(const Mat input){
    init = input.clone();
	//input.convertTo(init, CV_8UC3,alpha,beta);
    ThresholdProcess();
    ContourProcess();
}

void ImageProcess::ThresholdProcess(){
    cvtColor(init, gray, CV_BGR2GRAY);
    threshold(gray, Binary, thres_val, 255, THRESH_BINARY);
}

void ImageProcess::ContourProcess(){
    result_sort.clear();
    Binary.copyTo(Binary_nouse);
    findContours(Binary_nouse, contours, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    size_t size = contours.size();
    vector<Point> approx;
    vector<RMPoint> approx_result;
    Rect bound;
    RMPoint forsort;
    
    Point2f aff_input[4];
    
    for(int i=0;i<size;i++){
        approxPolyDP(contours[i], approx, 10, true);
        if(approx.size() == 4){
            bound = boundingRect(contours[i]);
            if(bound.area() > contour_min && bound.area() < contour_max && (float)bound.width/bound.height>1.0 && (float)bound.width/bound.height<2.0){
                approx_result.clear();
                
                for(int i=0;i<4;i++){
                    forsort.data(approx[i]);
                    approx_result.push_back(forsort);
                }
                std::sort(approx_result.begin(),approx_result.end());
                /*for(int i=0;i<4;i++){
                    aff_input[i] = Point2f(approx_result[i].x,approx_result[i].y);
                }*/
                aff_input[0] = Point2f(approx_result[0].x, approx_result[0].y);
                aff_input[1] = Point2f(approx_result[1].x, approx_result[1].y);
                aff_input[2] = Point2f(approx_result[2].x, approx_result[2].y);
                aff_input[3] = Point2f(approx_result[3].x, approx_result[3].y);
                Mat target = aff_process(gray, aff_input); // return mat
				resize(target,target,Size(targetsize,targetsize));
				//Mat FinalTarget = target(Rect(targetsize*0.4,targetsize*0.25,targetsize*0.2,targetsize*0.2));
                rectangle(init, bound, Scalar(255,0,255),2);
                /*Mat binary_target;
                threshold(target, binary_target, thres_detector, 255, THRESH_BINARY);
                vector<KeyPoint> keys;
                
                FastFeatureDetector fast(40);
                fast.detect(binary_target, keys);
                TarPos pos(bound.tl(),binary_target,static_cast<int>(keys.size()),bound);
                rectangle(init, bound, Scalar(255,0,255),2);*/
                
                //cout<<bound.tl()<<endl;
                TarPos pos(bound.tl(),target,0,0,bound);
                result_sort.push_back(pos);
            }
        }
    }
    if(result_sort.size() != 9){
        result = 0;
        return;
    }
    sort(result_sort.begin(),result_sort.end());
    int keypoint_size = INT_MAX;
    int total_size = INT_MAX;
    TString windowname;
    for(int i=0;i<9;i++){
        Mat binary_target;
        ++windowname;
		int wsize = 0;
		int csize = 0;
        //vector<KeyPoint> keys;
		binary_target = result_sort[i].data.clone();
		vector<vector<Point> > keys;
		
		threshold(binary_target, binary_target,thres_detector[i], 255, THRESH_BINARY);
		//Canny(binary_target,binary_target,50,200);
		Rect affrect = Rect_<int>(5,5,targetsize-10,targetsize-10);
		Mat TargetCal = binary_target(affrect);
		if(debug_flag){
			imshow(windowname.str, TargetCal);//binary_target(affrect)
		}
		for(int i=0;i<TargetCal.total();i++){
			if(TargetCal.at<uchar>(i) == 0) wsize++;
		}
		
		findContours(TargetCal,keys,CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE);//binary_target(affrect)
        csize = keys.size();
		
        //cout<<"size:"<<wsize<<endl;//keys.size()
        if(wsize < total_size){
            total_size = static_cast<int>(wsize);
        }
		if(csize < keypoint_size ){
			keypoint_size = csize;
		}
        result_sort[i].keysize = static_cast<int>(wsize); // key
        result_sort[i].csize = static_cast<int>(csize);   // contour
    }
    
    for(int i=0;i<9;i++){
        if(result_sort[i].keysize == total_size){
			if(result_sort[i].csize == keypoint_size){
		        rectangle(init, result_sort[i].target, Scalar(255,0,0),2);
				old_result = result;
		        result = i+1;
				/*if(old_result == result){
					if(old_count < 0) old_count = 0;
					old_count++;
					if(old_count>30) old_count = 30;
				}else{
					if(old_count == 0) old_count--;
					else old_count = 0;
				}
				if(old_count < 0) result = old_result;
				*/
		        break;
			}else{
				result = old_result;
			}
        }
    }
    //cout<<"pass"<<endl;
}

Mat ImageProcess::aff_process(const Mat input,Point2f aff_input[4]){
    Mat aff = getAffineTransform(aff_input,aff_val);
    Mat dst;
    warpAffine(input, dst, aff, Size(targetsize,targetsize));
    return dst;
}

}











