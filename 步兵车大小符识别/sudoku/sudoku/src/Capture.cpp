//
//  Capture.cpp
//  sudoku
//
//  Created by ding on 17/6/5.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "Capture.h"

namespace sudoku{

ImageInit::ImageInit():_id(0),_rows(480),_cols(640){
    calibration_switch = 0;
    capture.open(_id);
    if (!capture.isOpened()) {
        cout<<"Invalid device!"<<endl;
    } else {
        capture.set(CV_CAP_PROP_FRAME_HEIGHT , _rows);
        capture.set(CV_CAP_PROP_FRAME_WIDTH , _cols);
    }
}

ImageInit::ImageInit(int device,int rows,int cols){
    _id = device;
    _rows = rows;
    _cols = cols;
    calibration_switch = 0;
    capture.open(_id);
    if (!capture.isOpened()) {
        cout<<"Invalid device!"<<endl;
    } else {
        capture.set(CV_CAP_PROP_FRAME_HEIGHT , _rows);
        capture.set(CV_CAP_PROP_FRAME_WIDTH , _cols);
    }
}

ImageInit::ImageInit(const string &filename){
    calibration_switch = 0;
    capture.open(filename);
    if (!capture.isOpened()) {
        cout<<"Invalid input file!"<<endl;
    }
}

ImageInit::~ImageInit(){}

void ImageInit::Getimage(Mat &imgarray){
    capture>>img;
    if (calibration_switch == 1) {
        undistort(img, imgarray, camera_matrix, distortion_coefficients);
    } else {
        imgarray = img;
    }
}

void ImageInit::calibrate_coefficients(){
    calibration_switch = 1;
    FileStorage file_storage("/home/ubuntu/Desktop/sudoku_linux/out_camera_data.xml", FileStorage::READ);
    file_storage["Camera_Matrix"] >> camera_matrix;
    file_storage["Distortion_Coefficients"] >> distortion_coefficients;
    file_storage.release();
}

}
