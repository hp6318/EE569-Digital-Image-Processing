#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 

#include <string.h>

using namespace std;

double psnr(unsigned char *filtered,unsigned char *noiseFree,int size_x,int size_y){
    const int max=255;
    double mse=0;
    double temp=0;
    double output;
    const int divisor=size_x*size_y;
    for (int i=0;i<size_x;i++){
        for (int j=0;j<size_y;j++){
            temp=pow(*((filtered+i*size_y+j))-*((noiseFree+i*size_y+j)),2);
            mse=mse+temp;
        }
    }
    mse=mse/divisor;
    output=10*log10(pow(max,2)/mse);
    return output;
}

const double mean_3[3][3] = {
    {1, 2, 1},
    {2, 4, 2},
    {1, 2, 1}
};


//define a function to conduct various convolution
double convolution(unsigned char *img_padd,const double filter[3][3],int filter_size,int img_x,int img_y,int rows,int cols){
    int filter_x=0;
    int filter_y=0;
    double sum_val=0;  
    double mul_val=0;
	
    for (int i=img_x;i<img_x+filter_size;i++){
        for (int j=img_y;j<img_y+filter_size;j++){
			mul_val=*((img_padd+i*cols+j)) * filter[filter_x][filter_y];
            filter_y+=1;
            sum_val=sum_val+mul_val;
        }
        filter_x+=1;
        filter_y=0;
    }
    sum_val=sum_val/16;
    return sum_val;
}


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
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
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
    //code to padd image with reflected rows and colums
    const int padd_size_x=Size_x+2;
	const int padd_size_y=Size_y+2;
    unsigned char img_padd[padd_size_x][padd_size_y][BytesPerPixel];
    
    //copying the center core 
    for (int i=0;i<padd_size_x-2;i++){
        for (int j=0;j<padd_size_y-2;j++){
            img_padd[i+1][j+1][0]=Imagedata[i][j][0];
        }
    }
    //adding reflected column on left
    for (int i=0;i<padd_size_x-2;i++){
        img_padd[i][0][0]=Imagedata[i-1][1][0];
    }
    // //adding reflected column on right
    for (int i=0;i<padd_size_x-2;i++){
        img_padd[i+1][padd_size_y-1][0]=Imagedata[i][padd_size_y-4][0];
    }
    // //adding reflected row on top
    for (int i=0;i<padd_size_y;i++){
        img_padd[0][i][0]=img_padd[2][i][0];
    }
    // //adding reflected row on bottom
    for (int i=0;i<padd_size_y;i++){
        img_padd[padd_size_x-1][i][0]=img_padd[padd_size_x-3][i][0];
    }

    //code for convoultion with linear filter
	unsigned char img_denoised [Size_x][Size_y][1];
	int filter_size=3;
	
	for (int i=1;i<Size_x+1;i++){
		for (int j=1;j<Size_y+1;j++){
            double result = convolution((unsigned char *)img_padd,mean_3,3,i-1,j-1,padd_size_x,padd_size_y);
            img_denoised[i-1][j-1][0]=floor(result);
        }
    }
    cout << "The input image is" <<endl;
    int i,j;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)Imagedata[i][j][0]<<",";
		}
		cout<<endl;
    }

    cout << "The padded image is" <<endl;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)img_padd[i][j][0]<<",";
		}
		cout<<endl;
    }

    cout << "The processed image is" <<endl;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)img_denoised[i][j][0]<<",";
		}
		cout<<endl;
    }

    // Read noise-free image (filename specified by third argument) into image data matrix
	if (!(file=fopen(argv[3],"rb"))) {
		cout << "Cannot open file: " << argv[3] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size_x*Size_y*BytesPerPixel, file);
	fclose(file);

    cout << "The noise-free image is" <<endl;
	for (i=0;i<5;i++){
		for (j=0;j<5;j++){
			cout<< (int)Imagedata[i][j][0]<<",";
		}
		cout<<endl;
    }

    //compute the psnr score
    double score=psnr((unsigned char *)img_denoised,(unsigned char *)Imagedata,Size_x,Size_y);
    cout<<"PSNR score for Gaussian filter of kernel size=3 is : "<<score<<endl;


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(img_denoised, sizeof(unsigned char), Size_x*Size_y*1, file);
	fclose(file);

	return 0;
}

