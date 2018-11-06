#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <unistd.h>
using namespace cv;
using namespace std;

// vars for threshold
int const MAX_BINARY_VALUE = 255;

const int BINARY            = 0;
const int TO_ZERO           = 3;
const int TO_ZERO_INVERTED  = 4;

static Mat thresh(Mat *img, string window_name, string filename){
    int min_value = 50, max_value = 50;
    Mat min, max, thr;

    resize(*img, *img, Size(img->cols/3, img->rows/3));
    namedWindow( window_name, CV_WINDOW_AUTOSIZE); // Create a window to display results

    createTrackbar("min value", window_name, &min_value, MAX_BINARY_VALUE);
    createTrackbar("max value", window_name, &max_value, MAX_BINARY_VALUE);

    while(waitKey(50) != 'q'){
        threshold(*img, min, min_value, MAX_BINARY_VALUE, TO_ZERO); // set to 0 if less than min value
        threshold(min, max, max_value, MAX_BINARY_VALUE, TO_ZERO_INVERTED); // set to 0 if greater than max value
        threshold(max, thr, min_value, MAX_BINARY_VALUE, BINARY); // make numbers greater than min value white
        imshow(window_name, thr); // display results
    }

    imwrite("/Users/fangrl4ever/Desktop/" + filename, thr);
    return thr;
}

int main( int argc, char** argv )
{
    // variables
    Mat img_hsv, img_rgb, img_h, img_s, img_v;
    Mat thresh_h;
    Mat thresh_s;
    Mat thresh_v;
    img_rgb = imread("/Users/fangrl4ever/Desktop/test.png", 1);
    cvtColor(img_rgb, img_hsv, COLOR_BGR2HSV);
    const char* window_name = "Threshold Demo";

    // save H, S, and V values into separate images
    std::vector<Mat> channels;
    split(img_hsv, channels);
    img_h = channels[0];
    img_s = channels[1];
    img_v = channels[2];
    imwrite("/Users/fangrl4ever/Desktop/H.jpeg", img_h);
    imwrite("/Users/fangrl4ever/Desktop/S.jpeg", img_s);
    imwrite("/Users/fangrl4ever/Desktop/V.jpeg", img_v);

    thresh_h = thresh(&img_h, window_name, "H2.jpeg");
    thresh_s = thresh(&img_s, window_name, "S2.jpeg");
    thresh_v = thresh(&img_v, window_name, "V2.jpeg");

    return 0;
}
