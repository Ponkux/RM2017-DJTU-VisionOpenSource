//
//  Header.h
//  sudoku
//
//  Created by ding on 17/6/5.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef sudoku_Header_h
#define sudoku_Header_h

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <vector>
#include <opencv2/core/core.hpp>
#include <string>
#include <math.h>
#include <caffe/caffe.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <utility>

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using std::cout;
using std::endl;

typedef struct{
    int thresholdup;
    int thresholddown;
    int thresholdfindup;
    double alpha;
    double beta;
    int small_area_min;
    int small_area_max;
    int big_area_min;
    int big_area_max;
    int debug_flag;
	int debug_mode;
}InitParams;
 
template<typename T>
void CheckValue(const string &dataname,T dataold,T datanew){
    if(dataold != datanew){
        cout<<dataname<<":"<<datanew;
    }
}

#endif
