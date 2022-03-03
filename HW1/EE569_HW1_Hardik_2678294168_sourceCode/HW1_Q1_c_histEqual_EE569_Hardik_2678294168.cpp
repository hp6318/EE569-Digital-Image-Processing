#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>
#include <string.h>

using namespace std;
int clip(int value){
    int out=0;
    if (value>255){
        out=255;
    }
    else if (value<0){
        out=0;
    }
    else{
        out=floor(value);
    }
    return out;
}

int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
	
    // Define file pointer and variables
    FILE *file; 
	int BytesPerPixel=3;
	int Size_x =400;
	int Size_y =600;
	
	// Check for proper syntax
    if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}
	
	// Check if image is grayscale or color
	if (argc < 4){
		BytesPerPixel = 1; // default is grey image
	} 
	else {
		BytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		if (argc >= 5){
			Size_x = atoi(argv[4]);
		}
	}
	
	// Allocate image data array
	unsigned char Imagedata[Size_x][Size_y][3];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size_x*Size_y*3, file);
	fclose(file);
    
	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    //convert RGB to YUV color space using the given conversion in appendix
    unsigned char Imagedata_yuv[Size_x][Size_y][3];
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            Imagedata_yuv[i][j][0]=(0.257 * Imagedata[i][j][0]) + (0.504 * Imagedata[i][j][1]) + (0.098 * Imagedata[i][j][2]) + 16;
            Imagedata_yuv[i][j][1]=-(0.148 * Imagedata[i][j][0]) - (0.291 * Imagedata[i][j][1]) + (0.439 * Imagedata[i][j][2]) + 128;            
            Imagedata_yuv[i][j][2]=(0.439 * Imagedata[i][j][0]) - (0.368 * Imagedata[i][j][1]) - (0.071 * Imagedata[i][j][2]) + 128;
        }
    }
  
    
    double histogram[256]={0}; //0-255 intensity values
    //calculate histogram of the input image by summing no. of pixels for each intensity value
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)Imagedata_yuv[i][j][0]]=histogram[(int)Imagedata_yuv[i][j][0]]+1;
        }
    }

    //Compute PDF/PMF by dividing histogram values for each intensity by total pixels
    int total_px=Size_x*Size_y;
    double pdf[256]={0}; 
    for (int i=0;i<256;i++){
        pdf[i]=histogram[i]/total_px;
        //cout<<i<<"-"<<pdf[i]<<endl;
    }

    //compute cdf by cummulative addition of pdf with increasing value of x-intensity
    double cdf[256]={0};
    cdf[0]=pdf[0];
    for (int i=1;i<256;i++){
        cdf[i]=cdf[i-1]+pdf[i];
        //cout<<i<<"-"<<cdf[i]<<endl;
    }    
    
    //Create mapping rule x-->cdf*256 & combining y' with u,v channels
    //unsigned char contrast_a[Size_x][Size_y][3];
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            Imagedata[i][j][0]=floor(cdf[(int)Imagedata_yuv[i][j][0]]*255);
            Imagedata[i][j][1]=Imagedata_yuv[i][j][1];
            Imagedata[i][j][2]=Imagedata_yuv[i][j][2];
        }
    }
    

    //convert RGB to YUV color space using the given conversion in appendix
    //unsigned char Imagedata_rgb[Size_x][Size_y][3];
    
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            Imagedata_yuv[i][j][0]=clip(1.164*(Imagedata[i][j][0] - 16) + 1.596*(Imagedata[i][j][2] - 128));
            Imagedata_yuv[i][j][1]=clip(1.164*(Imagedata[i][j][0] - 16) - 0.813*(Imagedata[i][j][2] - 128)- 0.391*(Imagedata[i][j][1] - 128));            
            Imagedata_yuv[i][j][2]=clip(1.164*(Imagedata[i][j][0] - 16) + 2.018*(Imagedata[i][j][1] - 128));
        }
    }


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_yuv, sizeof(unsigned char), Size_x*Size_y*3, file);
	fclose(file);

	return 0;
}
