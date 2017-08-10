//
//  InitParam.cpp
//  sudoku
//
//  Created by ding on 17/6/10.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "InitParam.h"
#include "TString.h"

void WriteParam(const InitParams &params,FileStorage& fs)
{
    fs<<"thres1"<<params.thresholdup
    <<"thres2"<<params.thresholddown
    <<"thres3"<<params.thresholdfindup
    <<"alpha"<<params.alpha
    <<"beta"<<params.beta
    <<"small_min"<<params.small_area_min
    <<"small_max"<<params.small_area_max
    <<"big_min"<<params.big_area_min
    <<"big_max"<<params.big_area_max;
}

void InitParam(InitParams &params,const FileStorage& fs){
    fs["thres1"]>>params.thresholdup;
    fs["thres2"]>>params.thresholddown;
    fs["thres3"]>>params.thresholdfindup;
    fs["alpha"]>>params.alpha;
    fs["beta"]>>params.beta;
    fs["small_min"]>>params.small_area_min;
    fs["small_max"]>>params.small_area_max;
    fs["big_min"]>>params.big_area_min;
    fs["big_max"]>>params.big_area_max;
    fs["debug_flag"]>>params.debug_flag;
	fs["debug_mode"]>>params.debug_mode;
} 

void InitOldParam(sudokuold::Params &params,const FileStorage& fs){
    TString thresname;
	thresname<<"old_thres_detector_";
	fs["old_thres_val"]>>params.thres_val;
	fs["alpha"]>>params.alpha;
    fs["beta"]>>params.beta;
	for(int i=0;i<9;i++){
		++thresname;
		fs[thresname.str]>>params.thres_detector[i];
	}
	fs["old_contour_min"]>>params.contour_min;
	fs["old_contour_max"]>>params.contour_max;
	fs["old_targetsize"]>>params.targetsize;
	fs["debug_flag"]>>params.debug_flag;
}
