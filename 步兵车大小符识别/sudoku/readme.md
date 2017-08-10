@author:Peiran Ding
This is the major program for sudoku object detection mission in Robomasters2017 Final Match
Architecture:

                                        Initial Image
                                              | 
                                        Find 9 digits
                                              |
                                    caffe model (lenet) 
                {if has wrong prediction, predict by another model and compare}
                                              |
                                  Estimate Led letters region
                                              |
                                        Find 5 Leds
                                              |
                                Predict Led numbers(svm with HOG)
                                              |
                                        Logic Process
                                              |
                                        Final result
Parameters can be adjusted in Params.xml.
Please calibrate the camera before use.
Caffe models is trained for 1~9 without 0.

Serial Transform Part:
num:uchar pos:uchar
______________________________________________________
| 0  | 1  | 2  | 3  |
|0xA5|num |pos |0xFF|
______________________________________________________
Serial Receive Part:
mode:uchar
______________________________________________________
| 0  | 1  | 2  |
|0xC6|mode|0xFE|
______________________________________________________
DJTU TOE 
20170703
