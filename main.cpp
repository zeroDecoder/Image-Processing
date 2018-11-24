//
//  main.cpp
//  Add Filters
//
//  Created by Vivian Zhu on 11/17/18.
//  Copyright © 2018 Vivian Zhu. All rights reserved.
//

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <unistd.h>
#include <tuple>
#include "OpenFilter.hpp"
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
   Mat img_rgb;
    img_rgb = imread("/Users/fangrl4ever/Desktop/test.png", 1);

    Filter myFilter;

     myFilter.config(img_rgb);
    //setting up min and max values
    int key = 'i';
    int newKey;

    do
    {
        newKey = waitKey(100);
        if(newKey == 'h')
            key = newKey;
        else if(newKey == 's')
            key = newKey;
        else if(newKey == 'v')
            key = newKey;
        else if(newKey == 'i')
            key = 0;

        Mat res = myFilter.edgeDetect(&img_rgb);
        if(key == 'h')
            res = *myFilter.getH();
        else if (key == 's')
            res = *myFilter.getS();
        else if (key == 'v')
            res = *myFilter.getV();

        resize(res, res, Size(res.cols/3, res.rows/3));
        imshow("testWindow", res);
        
    }while(newKey != 'q');

    destroyWindow("testWindow");
    return 0;
}
