//
//  OpenFilter.cpp
//  Add Filters
//
//  Created by Vivian Zhu on 11/17/18.
//  Copyright © 2018 Vivian Zhu. All rights reserved.
//

#include "OpenFilter.hpp"

// constructor
Filter::Filter(void){
    this->h = new Mat(); // h is hue
    this->s = new Mat(); // s is saturation
    this->v = new Mat(); // v is value/brightness
    h_min = 40;
    h_max = 200;
    s_min = 40;
    s_max = 200;
    v_min = 40;
    v_max = 200;
}

// returns h pointer
Mat* Filter::getH()
{
    return this->h;
}

// returns s pointer
Mat* Filter::getS()
{
    return this->s;
}

// returns v pointer
Mat* Filter::getV()
{
    return this->v;
}

// allows the user to continuously edit the h, s, v min & max threhold values
void Filter::config(Mat img){
    createHSV(&img);
    // not used b/c setMinMax() needs to be called each time to change threshold values
    /*
    setMinMax(*this->h, this->h_min, this->h_max);
    setMinMax(*this->s, this->s_min, this->s_max);
    setMinMax(*this->v, this->v_min, this->v_max);
    */
    resize(img, img, Size(img.cols/3, img.rows/3));
    const char *windowLocal = "Test";

    waitKey(100);

    waitKey(100);
    imshow(windowLocal, img);

    createTrackbar("H min value", windowLocal, &h_min, Filter::MAX_BINARY_VALUE);
    createTrackbar("H max value", windowLocal, &h_max, Filter::MAX_BINARY_VALUE);

    createTrackbar("S min value", windowLocal, &s_min, Filter::MAX_BINARY_VALUE);
    createTrackbar("S max value", windowLocal, &s_max, Filter::MAX_BINARY_VALUE);

    createTrackbar("V min value", windowLocal, &s_min, Filter::MAX_BINARY_VALUE);
    createTrackbar("V max value", windowLocal, &s_min, Filter::MAX_BINARY_VALUE);
    
    waitKey(100);
}

/*
// sets the min & max threshold value for Mat object passed through
void Filter::setMinMax(Mat img, int theMin, int theMax)
{
    static char windowLocal[]= "a";
    int min_value = 50, max_value = 50;
    Mat min, max, thr;

    resize(img, img, Size(img.cols/3, img.rows/3));
    namedWindow( WINDOW_NAME, CV_WINDOW_AUTOSIZE); // Create a window to display results

    createTrackbar("min value", windowLocal, &min_value, Filter::MAX_BINARY_VALUE);
    createTrackbar("max value", windowLocal, &max_value, Filter::MAX_BINARY_VALUE);

    while(waitKey(50) != 'q'){
        threshold(img, min, min_value, Filter::MAX_BINARY_VALUE, Filter::TO_ZERO); // set to black if less than min value
        threshold(min, max, max_value, Filter::MAX_BINARY_VALUE, Filter::TO_ZERO_INVERTED); // set to black if greater than max value
        threshold(max, thr, min_value, Filter::MAX_BINARY_VALUE, Filter::BINARY); // set to white if greater than min value
        imshow(windowLocal, thr); // display results
    }
    windowLocal[0] +=1;
    img = thr;
    theMin = min_value;
    theMax = max_value;
}
*/

// uses takes in the min and max threshold values and returns thresholded image
void Filter::thresh(Mat *img, int min_value, int max_value)
{
    Mat min, max, thr;

    threshold(*img, min, min_value, MAX_BINARY_VALUE, TO_ZERO); // set to black if less than min value
    threshold(min, max, max_value, MAX_BINARY_VALUE, TO_ZERO_INVERTED); // set to black if greater than max value
    threshold(max, thr, min_value, MAX_BINARY_VALUE, BINARY); // set to white if greater than min value

    *img = thr;
}

void Filter::configAdapt(Mat img){
    createHSV(&img);

    resize(img, img, Size(img.cols/3, img.rows/3));
    const char *windowLocal = "Test";

    waitKey(100);

    waitKey(100);
    imshow(windowLocal, img);

    createTrackbar("H const value", windowLocal, &h_const, Filter::MAX_BINARY_VALUE);
    createTrackbar("S const value", windowLocal, &s_const, Filter::MAX_BINARY_VALUE);
    createTrackbar("V const value", windowLocal, &s_const, Filter::MAX_BINARY_VALUE);

    waitKey(100);
}

void Filter::adaptiveThresh(Mat *img, int the_const){
    Mat thr;

    adaptiveThreshold(*img, thr, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 11, (double)the_const);
    *img = thr;
}

// converts 1 HSV image to 3 images (one for each H, S, and V values)
void Filter::createHSV(Mat *img)
{
    // variables
    Mat img_hsv;

    cvtColor(*img, img_hsv, COLOR_BGR2HSV);

    // save H, S, and V values into separate images
    std::vector<Mat> channels;
    split(img_hsv, channels);
    *this->h = channels[0];
    *this->s = channels[1];
    *this->v = channels[2];
}

// takes thresholded h, s, and v images and stacks them, applying a blur,
// then uses Sobel for edge detection
Mat Filter::edgeDetect(Mat *img)
{
    Mat temp_img, stacked_img, edge_img;

    createHSV(img);

    /*thresh(this->h, h_min, h_max);
    thresh(this->s, s_min, s_max);
    thresh(this->v, v_min, v_max);*/

    adaptiveThresh(this->h, h_const);
    adaptiveThresh(this->s, s_const);
    adaptiveThresh(this->v, v_const);

    // stacking H, S, and V into one picture
    temp_img = *h & *s;
    stacked_img = temp_img & *v;

    // Sobel
    int ksize = 1;
    int scale = 4;
    int delta = 0;
    int ddepth = CV_16S;
    GaussianBlur(stacked_img, edge_img, Size(3, 3), 0, 0, BORDER_DEFAULT);
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(edge_img, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    Sobel(edge_img, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    convertScaleAbs(grad_x, abs_grad_x); // converting back to CV_8U
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, stacked_img);

    return stacked_img;
}
