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
#include <fstream>
#include <unistd.h>
#include <tuple>
#include "OpenFilter.hpp"
using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    Mat img_rgb; // image taken in from the user
    img_rgb = imread("/Users/fangrl4ever/Desktop/test.png", 1);

    Filter myFilter;

    // configures the min and max threshold values for each h, s, v value
    myFilter.config(img_rgb);

    int key = 'i';
    int newKey;

    // view each h, s, v value by typing "h", "s", or "v" respectively
    // view the edge detected image by typing "i"
    // when finished setting up values, type "q" to quit
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

    myFilter.writeHSV();

    destroyWindow("testWindow");
    return 0;
}
