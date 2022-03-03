
//EE559 ---  HW2_Q1b ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio Code --- g++ compiler 
//Project: Canny Edge Detection
//Arguments to be passed: 2
    //1) Input image file(raw)
    //2) Output image file(raw)

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <string.h>
#include <fstream>
#define _CRT_SECURE_NO_DEPRECATE

using namespace std;
using namespace cv;

void read(const char* filename, void* image_mat, int bpp, int size_x, int size_y) {
    FILE* file;
    if (!(file = fopen(filename, "r"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    fread(image_mat, sizeof(unsigned char), size_x * size_y * bpp, file);
    fclose(file);
}

void write(const char* filename, void* image_mat, int bpp, int size_x, int size_y) {
    FILE* file;
    if (!(file = fopen(filename, "w"))) {
        cout << "Cannot open file: " << filename << endl;
        exit(1);
    }
    fwrite(image_mat, sizeof(unsigned char), size_x * size_y * bpp, file);
    fclose(file);
}



//Mat src, src_gray;
//Mat dst, detected_edges;
//int lowThreshold = 0;  
//const int max_lowThreshold = 100; //gradient magnitude values lower than this are classified as no edge
//const int ratio = 3; // recommended to have higher threshold value in ratio of either 2:1 or 3:1
//const int kernel_size = 3; //sobel filter size 
//const char* window_name = "Edge Map";  //Display image window


//static void CannyThreshold(int, void*)
//{
//    blur(src_gray, detected_edges, Size(3, 3));
//    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * 3, kernel_size);
//    dst = Scalar::all(0);
//    src.copyTo(dst, detected_edges);
//    imshow(window_name, dst);
//}



int main(int argc, char** argv)
{
    const int Size_x = 321; //height
    const int Size_y = 481; //width
    const int Bpp = 3; //rgb

    //Allocate dynamic memory for input image container
    unsigned char* Imagedata = new (nothrow) unsigned char[Size_x * Size_y * Bpp];
    read(argv[1], Imagedata, Bpp, Size_x, Size_y);
    
    //Convert unsigned char array to opencv matrix object
    Mat input_img(Size_x, Size_y, CV_8UC3, Imagedata); 
    
    //delete[] Imagedata;

    //convert RGB to grayscale 
    Mat gray_img;
    cvtColor(input_img, gray_img, COLOR_RGB2GRAY);
    
    //blur the image with defined kernel size
    int kernel_size = 3;
    Mat img_blur;
    blur(gray_img, img_blur, Size(kernel_size, kernel_size));

    //Apply Canny edge
    Mat canny_img;
    Canny(img_blur, canny_img, 80, 200, 3, false); //low_thres=100, high_thres=250 kernel=3x3

    //reverse the px with edges=0, background=255
    for (int i = 0;i < Size_x;i++) {
        for (int j = 0;j < Size_y;j++) {
            if (canny_img.at<char>(i, j) == 0) {
                canny_img.at<char>(i, j) = 255;
            }
            else {
                canny_img.at<char>(i, j) = 0;
            }
        }
    }


    //Display the edge map.
    imshow("Canny_edges", canny_img);
    waitKey(0);
    destroyAllWindows();

    //write the output image 
    imwrite(argv[2], canny_img);






    //src = imread(argv[1]); // Load an image
    //imshow("input", src);
    //waitKey(0);
    //if (src.empty())
    //{
    //    std::cout << "Could not open or find the image!\n" << std::endl;
    //    std::cout << "Usage: " << argv[0] << " <Input image>" << std::endl;
    //    return -1;
    //}
    //dst.create(src.size(), src.type());
    //cvtColor(src, src_gray, COLOR_BGR2GRAY);
    //namedWindow(window_name, WINDOW_AUTOSIZE);
    //createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);
    //CannyThreshold(0, 0);
    //waitKey(0);

    //imwrite(argv[2], )
    return 0;
}