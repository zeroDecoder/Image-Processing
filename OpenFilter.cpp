//
//  OpenFilter.cpp
//  Add Filters
//
//  Created by Vivian Zhu on 11/17/18.
//  Copyright © 2018 Vivian Zhu. All rights reserved.
//

#include "OpenFilter.hpp"

// vars for threshold

Filter::Filter(void){
    this->h = new Mat();
    this->s = new Mat();
    this->v = new Mat();
    h_min = 40;
    h_max = 200;
    s_min = 40;
    s_max = 200;
    v_min = 40;
    v_max = 200;
}

Mat* Filter::getH()
{
    return this->h;
}

Mat* Filter::getS()
{
    return this->s;
}
Mat* Filter::getV()
{
    return this->v;
}

void Filter::config(Mat img){
    createHSV(&img);
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
        threshold(img, min, min_value, Filter::MAX_BINARY_VALUE, Filter::TO_ZERO); // set to 0 if less than min value
        threshold(min, max, max_value, Filter::MAX_BINARY_VALUE, Filter::TO_ZERO_INVERTED); // set to 0 if greater than max value
        threshold(max, thr, min_value, Filter::MAX_BINARY_VALUE, Filter::BINARY); // make numbers greater than min value white
        imshow(windowLocal, thr); // display results
    }
    windowLocal[0] +=1;
    img = thr;
    theMin = min_value;
    theMax = max_value;
}

void Filter::thresh(Mat *img, int min_value, int max_value)
{
    Mat min, max, thr;

    threshold(*img, min, min_value, MAX_BINARY_VALUE, TO_ZERO); // t to 0 if less than min value
    threshold(min, max, max_value, MAX_BINARY_VALUE, TO_ZERO_INVERTED); // set to 0 if greater than max value
    threshold(max, thr, min_value, MAX_BINARY_VALUE, BINARY); // make numbers greater than min value white

    *img = thr;
}

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

Mat Filter::edgeDetect(Mat *img)
{
    Mat temp_img, stacked_img, edge_img;

    createHSV(img);

    thresh(this->h, h_min, h_max);
    thresh(this->s, s_min, s_max);
    thresh(this->v, v_min, v_max);

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
