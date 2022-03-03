#include "opencv2/core.hpp"
#include <vector>       // std::vector
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>
#include <string.h>

using namespace std;
using namespace cv;

//unsigned char Imagedata[400][600][3];

int main(int argc, char** argv)
{
    //FILE* file;
    //const int BytesPerPixel = 3;
    //const int Size_x = 400;
    //const int Size_y = 600;

    //// Check for proper syntax
    //if (argc < 3) {
    //    cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
    //    cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
    //    return 0;
    //}


    //// Allocate image data array
    ////unsigned char Imagedata[Size_x][Size_y][BytesPerPixel];

    //// Read image (filename specified by first argument) into image data matrix
    //if (!(file = fopen(argv[1], "rb"))) {
    //    cout << "Cannot open file: " << argv[1] << endl;
    //    exit(1);
    //}
    //fread(Imagedata, sizeof(unsigned char), Size_x * Size_y * BytesPerPixel, file);
    //fclose(file);

    //Mat rgb_image = Mat(Size_x, Size_y, CV_8UC1, (unsigned char *) Imagedata);

    // READ RGB color image and convert it to YUV
    Mat bgr_image = imread("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\Taj_Mahal_try.jpg");
    Mat YUV_image;
    cvtColor(bgr_image, YUV_image, COLOR_BGR2YUV);

    // Extract the y channel
    vector<Mat> yuv_planes(3);
    split(YUV_image, yuv_planes);  // now we have the Y channel in yuv_planes[0]

    // apply the CLAHE algorithm to the Y channel
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(4);
    Mat dst;
    clahe->apply(yuv_planes[0], dst);

    // Merge the the color planes back into an yuv image
    dst.copyTo(yuv_planes[0]);
    merge(yuv_planes, YUV_image);

    // convert back to RGB
    Mat image_clahe;
    cvtColor(YUV_image, image_clahe, COLOR_YUV2BGR);

    // display the results  (you might also want to see YUV_planes[0] before and after).
    imshow("image original", bgr_image);
    imshow("image CLAHE", image_clahe);

    imwrite("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\Taj_Mahal_hazeRemoved.jpg", image_clahe);
    waitKey();
}