#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>
#include <string.h>

using namespace std;
int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
	// Define file pointer and variables
	FILE *file; 
	int BytesPerPixel;
	int Size_x =256;
	int Size_y =256;
	
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
	unsigned char Imagedata[Size_x][Size_y][BytesPerPixel];

	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size_x*Size_y*BytesPerPixel, file);
	fclose(file);
	
	

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    double histogram[256]={0}; //0-255 intensity values
    //calculate histogram of the input image by summing no. of pixels for each intensity value
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)Imagedata[i][j][0]]=histogram[(int)Imagedata[i][j][0]]+1;
        }
    }

    //export the histogram values to csv file for further use in python for plotting
    fstream fout;
    fout.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\1b_methodA_hist.csv", ios::out | ios::app);
    
    cout<<"Histogram of input image is "<<endl;
    for (int i=0;i<256;i++){
        cout<<i<<" - "<<histogram[i]<<endl;
        fout<<i<<","<<histogram[i]<<"\n";
    }
    fout.close();


    // int sum=0;
    // for(int i=0;i<256;i++){
    //     sum=sum+histogram[i];
    //  }
    // cout<<"Sum = "<<sum<<endl;


    //Compute PDF/PMF by dividing histogram values for each intensity by total pixels
    int total_px=Size_x*Size_y;
    double pdf[256]={0}; 
    for (int i=0;i<256;i++){
        pdf[i]=histogram[i]/total_px;
        cout<<i<<"-"<<pdf[i]<<endl;
    }

    //compute cdf by cummulative addition of pdf with increasing value of x-intensity
    double cdf[256]={0};
    cdf[0]=pdf[0];
    for (int i=1;i<256;i++){
        cdf[i]=cdf[i-1]+pdf[i];
        cout<<i<<"-"<<cdf[i]<<endl;
    }    

    //Create mapping rule x-->cdf*256
    unsigned char contrast_a[Size_x][Size_y][1];
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            contrast_a[i][j][0]=floor(cdf[(int)Imagedata[i][j][0]]*255);
        }
    }
    
    //export new and old intensities value pair to csv file for plotting transfer function
    fout.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\1b_methodA_transferFun.csv", ios::out | ios::app);
    for (int i=0;i<255;i++){
        fout<<i<<","<<floor(cdf[i]*255)<<"\n";
    }
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(contrast_a, sizeof(unsigned char), Size_x*Size_y*1, file);
	fclose(file);

	return 0;
}
