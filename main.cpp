#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
using namespace cv;
using namespace std;

// vars for threshold
int THRESHOLD_VALUE = 48;
int const MAX_BINARY_VALUE = 255;

static void Threshold_Demo(Mat *the_src, Mat *the_dst, int threshold_type)
{
    threshold(*the_src, *the_dst, THRESHOLD_VALUE, MAX_BINARY_VALUE, threshold_type);
}

int main( int argc, char** argv )
{
    Mat image, src, src_gray;
    Mat grad, grad2, grad_dest;
    const String window_name = "Edge Detection";
    int ksize = 1;
    int scale = 4;
    int delta = 0;
    int ddepth = CV_16S;
    String imageName("/Users/fangrl4ever/Downloads/boxes.jpeg");

    image = imread( imageName, IMREAD_COLOR ); // Load an image

    // check for error
    if( image.empty() )
    {
        printf("Error opening image: %s\n", imageName.c_str());
        return 1;
    }

    // Sobel
    GaussianBlur(image, src, Size(3, 3), 0, 0, BORDER_DEFAULT);
    cvtColor(src, src_gray, COLOR_BGR2GRAY); // Convert the image to grayscale
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    Sobel(src_gray, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
    Sobel(src_gray, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);

    convertScaleAbs(grad_x, abs_grad_x); // converting back to CV_8U
    convertScaleAbs(grad_y, abs_grad_y);
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    imshow(window_name, grad);
    imwrite("/Users/fangrl4ever/Downloads/test.jpeg", grad);

    // Blur
    grad2 = grad.clone();
    for (int i = 1; i < 5; i = i + 2)
    {
        blur(grad, grad2, Size( i, i ), Point(-1,-1));
    }
    imwrite("/Users/fangrl4ever/Downloads/step2.jpeg", grad2);

    // Threshold
    Threshold_Demo(&grad, &grad_dest, 0); // Call the function to initialize
    imwrite("/Users/fangrl4ever/Downloads/test2.jpeg", grad_dest);

    return 0;
}
