#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>

#include <string.h>

using namespace std;
double noise[512][768][3]={0};
unsigned char noisefree[512][768][3];

void export_(string filename,double PDF[256]){
    fstream fout;
    fout.open(filename, ios::out | ios::app);
    for(int i=0;i<256;i++){
        fout<<i<<","<<PDF[i]<<"\n";
    }
    fout.close();

}
 

int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
    // Define file pointer and variables
	FILE *file; 
	int BytesPerPixel=3;
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

    // Read noise-free image (filename specified by third argument) into image data matrix
	//unsigned char noisefree[Size_x][Size_y][BytesPerPixel];
	
	if (!(file=fopen(argv[2],"rb"))) {
		cout << "Cannot open file: " << argv[2] <<endl;
		exit(1);
	}
	fread(noisefree, sizeof(unsigned char), Size_x*Size_y*BytesPerPixel, file);
	fclose(file);

    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            noise[i][j][0]=Imagedata[i][j][0]-noisefree[i][j][0]; //R
            noise[i][j][1]=Imagedata[i][j][1]-noisefree[i][j][1]; //G
            noise[i][j][2]=Imagedata[i][j][2]-noisefree[i][j][2]; //B
        }
    }
    //R channel
    double histogram[256]={0}; //0-255 intensity values
    //calculate histogram of the noise by summing no. of pixels for each intensity value
   
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)noise[i][j][0]]=histogram[(int)noise[i][j][0]]+1; //R
        }
    }
    int total_px=Size_x*Size_y;
    double pdf[256]={0}; 
    for (int i=0;i<256;i++){
        pdf[i]=histogram[i]/total_px;
        cout<<i<<"-"<<pdf[i]<<endl;
    }
    //export the histogram values to csv file for further use in python for plotting
    export_("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\2d_noise_R.csv",pdf);
    

    //G channel
    histogram[256]={0};
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)noise[i][j][1]]=histogram[(int)noise[i][j][1]]+1; //G
        }
    }
    //export the histogram values to csv file for further use in python for plotting
    export_("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\2d_noise_G.csv", histogram);

    
    //B channel
    histogram[256]={0};
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[(int)noise[i][j][2]]=histogram[(int)noise[i][j][2]]+1; //B
        }
    }
    //export the histogram values to csv file for further use in python for plotting
    export_("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\2d_noise_B.csv", histogram);

	return 0;
}

