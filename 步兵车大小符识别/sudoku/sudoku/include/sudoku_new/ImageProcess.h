//
//  ImageProcess.h
//  sudoku
//
//  Created by ding on 17/6/5.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#ifndef __sudoku__ImageProcess__
#define __sudoku__ImageProcess__

#include "Header.h"


using namespace cv;

namespace sudoku{
    

class SortRects{
public:
    SortRects(Rect input){
        x=input.tl().x;
        y=input.tl().y;
        height = input.height;
        width  = input.width;
    }
    int x,y,height,width;
    bool operator < (const SortRects &s)const{return x< s.x;}
};


class SortSudokus{
public:
    SortSudokus(Rect input){
        x=input.tl().x;
        y=input.tl().y;
        height = input.height;
        width  = input.width;
    }
    int x,y,height,width;
    bool operator < (const SortSudokus &s)const{
        if (s.y - y > 40) {
            return true;
        }else if (s.x - x > 60){
            return true;
        }
        return false;
    }
};




class ImageProcess{
public:
    ImageProcess();
    ImageProcess(const InitParams &params);
    ~ImageProcess(){};
    void ParamsFresh(const InitParams &params);
    void process(Mat input);
    void dataout(Mat &output,Mat &outgrayup,Mat &outgraydown,Mat &thresholdimg);
private:
    void Process();
    void ThresholdProcess();
    void ContourProcess();
    
public:
    Mat grayup,graydown,thresdown,thres4led;
    Mat inputgray;
    Mat imginit,img,brighted;
    Mat imgup,imgdown;
    Mat LedRoi,RoiBinary,RoiBinary_nouse;
    vector<SortRects> Lednums;
    vector<SortSudokus> sudokus;
    vector<Rect> LedArea;
    vector<Rect> SudokuArea;
	vector<SortRects> old_Lednums;
    vector<SortSudokus> old_sudokus;
    vector<Rect> old_LedArea;
    vector<Rect> old_SudokuArea;
    
private:
    Mat contour_nouse;
    int thres1,thres2;//thres1:find small thres2:find big thres3:use for predict led
    double alpha,beta;
    int small_min,small_max,big_min,big_max;
    vector<vector<Point> > contours,smallcontours;
    int debug_flag;
	int lostflag;
};
    
}


#endif /* defined(__sudoku__ImageProcess__) */
