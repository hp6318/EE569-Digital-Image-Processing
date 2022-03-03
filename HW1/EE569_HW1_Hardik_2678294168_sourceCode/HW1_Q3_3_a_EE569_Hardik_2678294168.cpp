#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include<time.h>
#include <string.h>

using namespace std;

unsigned char Imagedata[512][768][1];
unsigned char img_padd[516][772][1];

int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
    //cout<<1<<endl;
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
	//unsigned char Imagedata[Size_x][Size_y][BytesPerPixel];
    //cout<<2<<endl;
	// Read denoised image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), Size_x*Size_y*BytesPerPixel, file);
	fclose(file);
	
    //cout<<3<<endl;

	

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
    //code to padd image with reflected rows and colums
    const int padd_size_x=Size_x+4;
	const int padd_size_y=Size_y+4;
    //unsigned char img_padd[padd_size_x][padd_size_y][BytesPerPixel];
    //cout<<4<<endl;
    //copying the center core 
    for (int i=0;i<padd_size_x-4;i++){
        for (int j=0;j<padd_size_y-4;j++){
            img_padd[i+2][j+2][0]=Imagedata[i][j][0];
        }
    }
    //cout<<5<<endl;
    //adding first reflected column on left
    for (int i=0;i<padd_size_x;i++){
        img_padd[i+1][1][0]=Imagedata[i-1][1][0];
    }
    //cout<<'a'<<endl;
    
    //adding second reflected column on left
    for (int i=0;i<padd_size_x;i++){
        img_padd[i+1][0][0]=Imagedata[i-1][2][0];
        
    }
    //cout<<'b'<<endl;

    //adding 2 reflected columns on right
    for (int i=0;i<padd_size_x;i++){
        img_padd[i+1][padd_size_y-2][0]=Imagedata[i-1][padd_size_y-6][0];
        img_padd[i+1][padd_size_y-1][0]=Imagedata[i-1][padd_size_y-7][0];

    }
    //cout<<'c'<<endl;

    //adding 2 reflected rows on top
    for (int i=0;i<padd_size_y;i++){
        img_padd[1][i][0]=img_padd[3][i][0];
        img_padd[0][i][0]=img_padd[4][i][0];

    }
    //cout<<'d'<<endl;

    //adding 2 reflected rows on bottom
    for (int i=0;i<padd_size_y;i++){
        img_padd[padd_size_x-2][i][0]=img_padd[padd_size_x-4][i][0];
        img_padd[padd_size_x-1][i][0]=img_padd[padd_size_x-5][i][0];

    }
    //cout<<6<<endl;
    //code for convoultion with linear filter
	unsigned char img_glass [Size_x][Size_y][1];
	//cout<<7<<endl;
    srand(time(0));
	for (int i=2;i<Size_x+2;i++){
		for (int j=2;j<Size_y+2;j++){
            int row_idx=rand()%5-2;
            int col_idx=rand()%5-2;
            img_glass[i-2][j-2][0]=img_padd[i+row_idx][j+col_idx][0];
        }
    }


	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(img_glass, sizeof(unsigned char), Size_x*Size_y*1, file);
	fclose(file);
	return 0;
}

