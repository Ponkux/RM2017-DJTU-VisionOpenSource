//
//  Capture.h
//  sudoku
//
//  Created by ding on 17/6/5.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef __sudoku__Capture__
#define __sudoku__Capture__

#include <stdio.h>
#include "Header.h"

using namespace cv;

namespace sudoku{
    
class ImageInit {
public:
    ImageInit();
    ImageInit(int device,int rows,int cols);
    ImageInit(const string &filename);
    ~ImageInit();
    void Getimage(Mat &imgarray);
    void calibrate_coefficients(); // Use it if camera need to be calibrated. Otherwise do not.
private:
    Mat img;
private:
    int _id;
    int _rows;
    int _cols;
    int calibration_switch;
    VideoCapture capture;
    Mat camera_matrix = Mat(3, 3, CV_32FC1);
    Mat distortion_coefficients;
};

}
#endif /* defined(__sudoku__Capture__) */
