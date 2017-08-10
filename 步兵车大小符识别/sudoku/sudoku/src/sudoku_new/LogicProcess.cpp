//
//  LogicProcess.cpp
//  sudoku
//
//  Created by ding on 17/6/7.
//  Copyright (c) 2017年 ding. All rights reserved.
//

#include "sudoku_new/LogicProcess.h"
namespace sudoku{
    
std::pair<int,int> LogicProcess::process(Confirm &confirm,vector<int> Ledsorted,vector<int> HandWritingsorted)
{
    if(confirm.Led == false || confirm.HandWriting == false || Ledsorted.size() != 5 || HandWritingsorted.size() != 9){ // Target not clear
		if(lostflag>100){
		    result.first = 0;
		    result.second = 0;
		    flag = 1;
			LedNums.clear();
		}else{
			lostflag++;
		}
        return result;
    }
	lostflag = 0;
    // Led 5 HandWriting 9
    if(flag == 0) flag = 1;
    if(LedNums.size() == 0){ 
        LedNums = Ledsorted; // Initialize
    }else{
        int count = 0;
        for(int i=0;i<5;i++){
            if(LedNums[i] != Ledsorted[i]) count++;
        }
        if(count > 1){ // LED change.Mission restart
            LedNums = Ledsorted;
			HandWritingNums = HandWritingsorted;
            flag = 1;
        }
        	if(HandWritingNums.size() == 0){ // Initialize
                HandWritingNums = HandWritingsorted;
				flag = 1;
            }else{
                int changecount = 0;
                for(int i=0;i<9;i++){
                    if(HandWritingNums[i] != HandWritingsorted[i]) changecount++;
                }
                if(changecount > 3){ // HandWriting digits change
                    flag++;
                    startflag = 0;
                    HandWritingNums = HandWritingsorted;
                    cout<<"saved:";
                    for(int i=0;i<9;i++){
                        cout<<HandWritingNums[i]<<" ";
                    }
                    cout<<endl;
					cout<<"flag:"<<flag<<"target:"<<result.second<<endl;
                }
            }
        
    }
    result.first = flag; // ?st number
	
    int target;
    if(flag > 5 || flag == 0){
        target = 0;
    }else{
        target = Ledsorted[flag-1];
    }
    
    if(target == 0){
        result.second = 0;
        return result;
    }
    
    int ifsame[9];
    for(int i=0;i<9;i++){
        ifsame[i] = HandWritingsorted[i];
    }
    std::sort(ifsame,ifsame+9);
    int samenumber = 0;
    for(int i=0;i<8;i++){
        if(ifsame[i]==ifsame[i+1]){
            samenumber = ifsame[i];
            break;
        }
    }
    
    if(samenumber != target){
        samenumber = 0;
    }else{
        cout<<"Target may miss at digit:"<<samenumber<<endl;
    }
    for(int i=0;i<9;i++){
        if(samenumber == 0){
            if(HandWritingsorted[i] == target){
                result.second = i+1;
                break;
            }
        }else{
            if(HandWritingsorted[i] == target){
                result.second = i+1;
                break;
            }
        }
    }
    
    return result;
}

void LogicProcess::DrawTarget(cv::Mat &input,vector<cv::Rect> targets){
    if(targets.size() == 9 && result.second != 0){
        cv::rectangle(input,targets[result.second-1], cv::Scalar(255,0,0),2);
    }
}

}




