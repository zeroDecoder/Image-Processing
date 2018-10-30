// Download an image and add filters to it

#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using std::cout;

// vars for threshold
int threshold_value = 220;
int threshold_type = 1; // threshold to 0
int const max_binary_value = 255;

// vars for blur
int MAX_KERNEL_LENGTH = 31;

Mat src, src_gray, dst, dst2;
const char* window_name = "Threshold Demo";

static void Threshold_Demo( int, void* )
{
    threshold( src_gray, dst, threshold_value, max_binary_value, threshold_type );
    imshow( window_name, dst );
}
int main( int argc, char** argv )
{
    String imageName("/Users/fangrl4ever/Downloads/boxes.jpeg"); // by default
    if (argc > 1)
    {
        imageName = argv[1];
    }
    src = imread( imageName, IMREAD_COLOR ); // Load an image
    if (src.empty())
    {
        cout << "Cannot read image: " << imageName << std::endl;
        return -1;
    }
    cvtColor( src, src_gray, COLOR_BGR2GRAY ); // Convert the image to Gray
    namedWindow( window_name, WINDOW_AUTOSIZE ); // Create a window to display results
    Threshold_Demo( 0, 0 ); // Call the function to initialize
    imwrite("/Users/fangrl4ever/Downloads/step1.jpeg", dst );
    waitKey(50);

    // blur code
    dst2 = dst.clone();
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
        blur( dst, dst2, Size( i, i ), Point(-1,-1) );
    }
    imwrite("/Users/fangrl4ever/Downloads/step2.jpeg", dst2);
    return 0;

}

