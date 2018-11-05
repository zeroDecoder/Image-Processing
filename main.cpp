#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <unistd.h>
using namespace cv;
using namespace std;

// vars for threshold
int const MAX_BINARY_VALUE = 255;

static void Threshold(Mat *the_src, Mat *the_dst, int threshold_type, int threshold_value)
{
    threshold(*the_src, *the_dst, threshold_value, MAX_BINARY_VALUE, threshold_type);
}

int main( int argc, char** argv )
{
    // variables
    Mat img_hsv, img_rgb, img_h, img_s, img_v;
    Mat thresh_h, min_h, max_h, thresh_h2, thresh_h3;
    Mat thresh_s, thresh_s2, thresh_s3;
    Mat thresh_v, thresh_v2, thresh_v3;
    img_rgb = imread("/Users/fangrl4ever/Desktop/test.png", 1);
    cvtColor(img_rgb, img_hsv, COLOR_BGR2HSV);
    int min_value = 50, max_value = 50;
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

//TRACKBAR STUFF
    resize(img_h, img_h, Size(img_h.cols/3, img_h.rows/3));
    namedWindow( window_name, CV_WINDOW_AUTOSIZE); // Create a window to display results
    resizeWindow(window_name, 800, 600);
    createTrackbar("min value", window_name, &min_value, 300);
    createTrackbar("max value", window_name, &max_value, 300);
    while(waitKey(50) != 'q'){
        Threshold(&img_h, &min_h, 3, min_value); // set to 0 if less than min value
        Threshold(&min_h, &max_h, 4, max_value); // set to 0 if greater than max value
        Threshold(&max_h, &thresh_h, 0, min_value); // make numbers greater than min value white
        imshow(window_name,thresh_h); // display results
        //resizeWindow(window_name, 800, 600);
    }
//END TRACKBAR STUFF

    /*
    Threshold(&img_h, &thresh_h, 3, 13);
    Threshold(&thresh_h, &thresh_h2, 4, 18);
    Threshold(&thresh_h2, &thresh_h3, 0, 13);
    imwrite("/Users/fangrl4ever/Desktop/H2.jpeg", thresh_h3);

    Threshold(&img_s, &thresh_s, 3, 160);
    Threshold(&thresh_s, &thresh_s2, 4, 180);
    Threshold(&thresh_s2, &thresh_s3, 0, 160);
    imwrite("/Users/fangrl4ever/Desktop/S2.jpeg", thresh_s3);

    Threshold(&img_v, &thresh_v, 3, 180);
    Threshold(&thresh_v, &thresh_v2, 4, 250);
    Threshold(&thresh_v2, &thresh_v3, 0, 160);
    imwrite("/Users/fangrl4ever/Desktop/V2.jpeg", thresh_v3);
     */

    return 0;
}
