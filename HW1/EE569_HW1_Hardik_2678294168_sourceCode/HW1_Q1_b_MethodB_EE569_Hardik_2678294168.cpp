#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <fstream>
#include <string.h>

using namespace std;
int main(int argc, char *argv[])

//MethodB, edges are more broader and clear. 

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
    fout.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\1b_methodB_hist.csv", ios::out | ios::app);
    
    cout<<"Histogram of input image is "<<endl;
    for (int i=0;i<256;i++){
        cout<<i<<" - "<<histogram[i]<<endl;
        fout<<i<<","<<histogram[i]<<"\n";
    }
    fout.close();

    //calculate bin size
    int total_px=Size_x*Size_y;
    int buck_size=(int)total_px/256; // (256*256)/256
    cout<<"bucket size is"<<" "<<buck_size<<endl;

    unsigned char contrast_b[Size_x][Size_y][1]; //enhanced image storage 
    
    int temp_size=0;
    int temp_color=0;
    for (int intensity=0;intensity<256;intensity++){
        for (int i=0;i<Size_x;i++){
            for (int j=0;j<Size_y;j++){
                if ((int)Imagedata[i][j][0]==intensity){
                    if (temp_size<buck_size){
                        contrast_b[i][j][0]=temp_color;
                        temp_size=temp_size+1;
                    }
                    else{
                        temp_color=temp_color+1;
                        contrast_b[i][j][0]=temp_color;
                        temp_size=1;
                    }                   
                }
            }
        }
    }

    double histogram_output[256]={0}; //0-255 intensity values
    //calculate histogram of the output image by summing no. of pixels for each intensity value
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram_output[(int)contrast_b[i][j][0]]=histogram_output[(int)contrast_b[i][j][0]]+1;
        }
    }

    //export the enhanced image histogram values to csv file for further use in python for plotting
    fstream fout2;
    fout2.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\1b_methodB_hist_enhanced.csv", ios::out | ios::app);
    cout<<"Histogram of output image is "<<endl;
    for (int i=0;i<256;i++){
        cout<<i<<" - "<<histogram_output[i]<<endl;
        fout2<<i<<","<<histogram_output[i]<<"\n";        
    }
    fout2.close();


    //export the cumulative histogram values of original and enhanced images.
    int cumulative_enhanced[256]={0};
    int cumulative_ori[256]={0};
    cumulative_enhanced[0]=histogram_output[0];
    cumulative_ori[0]=histogram[0];
    for (int i=1;i<256;i++){
        cumulative_ori[i]=cumulative_ori[i-1]+histogram[i];
        cumulative_enhanced[i]=cumulative_enhanced[i-1]+histogram_output[i];
    }

    fstream fout3;
    fout3.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW1\\results\\1b_methodB_cumm.csv", ios::out | ios::app);
    for (int i=0;i<256;i++){
        fout3<<i<<","<<(int)cumulative_ori[i]<<","<<(int)cumulative_enhanced[i]<<"\n";
    }
    fout3.close();


	// Write output image(emhanced) data (filename specified by second argument) from output image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(contrast_b, sizeof(unsigned char), Size_x*Size_y*1, file);
	fclose(file);

	return 0;
}
