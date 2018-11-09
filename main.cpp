#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <unistd.h>
#include <tuple>
using namespace cv;
using namespace std;

// vars for threshold
int const MAX_BINARY_VALUE = 255;
const int BINARY            = 0;
const int TO_ZERO           = 3;
const int TO_ZERO_INVERTED  = 4;
static int h_min, h_max, s_min, s_max, v_min, v_max;

struct MinMax
{
    int min, max;
};

struct HSV
{
    Mat h, s, v;
};

MinMax static setMinMax(Mat img, string window_name)
{
    int min_value = 50, max_value = 50;
    Mat min, max, thr;

    resize(img, img, Size(img.cols/3, img.rows/3));
    namedWindow( window_name, CV_WINDOW_AUTOSIZE); // Create a window to display results

    createTrackbar("min value", window_name, &min_value, MAX_BINARY_VALUE);
    createTrackbar("max value", window_name, &max_value, MAX_BINARY_VALUE);

    while(waitKey(50) != 'q'){
        threshold(img, min, min_value, MAX_BINARY_VALUE, TO_ZERO); // set to 0 if less than min value
        threshold(min, max, max_value, MAX_BINARY_VALUE, TO_ZERO_INVERTED); // set to 0 if greater than max value
        threshold(max, thr, min_value, MAX_BINARY_VALUE, BINARY); // make numbers greater than min value white
        imshow(window_name, thr); // display results
    }

    return {min_value, max_value};
}


static Mat thresh(Mat *img, int min_value, int max_value)
{
    Mat min, max, thr;

    threshold(*img, min, min_value, MAX_BINARY_VALUE, TO_ZERO); // set to 0 if less than min value
    threshold(min, max, max_value, MAX_BINARY_VALUE, TO_ZERO_INVERTED); // set to 0 if greater than max value
    threshold(max, thr, min_value, MAX_BINARY_VALUE, BINARY); // make numbers greater than min value white

    return thr;
}


HSV static createHSV(Mat *img)
{
    // variables
    Mat img_hsv, img_h, img_s, img_v;

    cvtColor(*img, img_hsv, COLOR_BGR2HSV);

    // save H, S, and V values into separate images
    std::vector<Mat> channels;
    split(img_hsv, channels);
    img_h = channels[0];
    img_s = channels[1];
    img_v = channels[2];

    return {img_h, img_s, img_v};
}


static Mat edgeDetect(Mat *img_h, Mat *img_s, Mat *img_v)
{
    Mat thresh_h, thresh_s, thresh_v, temp_img, stacked_img, edge_img;

    thresh_h = thresh(&*img_h, h_min, h_max);
    thresh_s = thresh(&*img_s, s_min, s_max);
    thresh_v = thresh(&*img_v, v_min, v_max);
    imwrite("/Users/fangrl4ever/Desktop/thresh_h.png",thresh_h);
    imwrite("/Users/fangrl4ever/Desktop/thresh_s.png",thresh_s);
    imwrite("/Users/fangrl4ever/Desktop/thresh_v.png",thresh_v);

    // stacking H, S, and V into one picture
    temp_img = thresh_h & thresh_s;
    stacked_img = temp_img & thresh_v;
    imwrite("/Users/fangrl4ever/Desktop/b4blur.png",stacked_img);

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


int main( int argc, char** argv )
{
    Mat img_rgb, h, s, v;
    img_rgb = imread("/Users/fangrl4ever/Desktop/test.png", 1);
    const char* window_name = "Threshold Demo";

    auto hsv = createHSV(&img_rgb);
    h = hsv.h;
    s = hsv.s;
    v = hsv.v;

    // setting up min and max values
    auto hmm = setMinMax(h, window_name);
    auto smm = setMinMax(s, window_name);
    auto vmm = setMinMax(v, window_name);

    h_min = hmm.min;
    h_max = hmm.max;
    s_min = smm.min;
    s_max = smm.max;
    v_min = vmm.min;
    v_max = vmm.max;

    imwrite("/Users/fangrl4ever/Desktop/stacked.png", edgeDetect(&h, &s, &v));
}
