#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>

#include <string.h>

using namespace std;
double noise[768][512]={0};

int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
	// Define file pointer and variables
	FILE *file; 
	int BytesPerPixel=1;
	int Size_x =512 ;
	int Size_y =768;
	
	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw noisefree_image.raw " << endl;
		return 0;
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
    
    cout << "The input image is" <<endl;
    int i,j;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)Imagedata[i][j][0]<<",";
		}
		cout<<endl;
    }

    // Read noise-free image (filename specified by third argument) into image data matrix
	unsigned char noisefree[Size_x][Size_y][BytesPerPixel];

	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(noisefree, sizeof(unsigned char), Size_x*Size_y*BytesPerPixel, file);
	fclose(file);

    cout << "The noise-free image is" <<endl;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)Imagedata[i][j][0]<<",";
		}
		cout<<endl;
    }
   
    
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            noise[i][j]=Imagedata[i][j][0]-noisefree[i][j][0];
        }
    }
   
    double histogram[512]={0}; //-255 to 255 intensity values
    //calculate histogram of the noise by summing no. of pixels for each intensity value
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)noise[i][j]]=histogram[(int)noise[i][j]]+1;
        }
    }
   
    //export the histogram values to csv file for further use in python for plotting
    fstream fout;
    fout.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\2a_noise_final.csv", ios::out | ios::app);
    
    cout<<"Histogram of noise is "<<endl;
    for (int i=-256;i<256;i++){
        //cout<<i<<" - "<<histogram[i]<<endl;
        fout<<i<<","<<histogram[i]<<"\n";
    }
    fout.close();


	return 0;
}

