#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 

#include <string.h>

using namespace std;

const double vertical_half[3][3] = {
    {0.00, 0.50, 0.00},
    {0.00, 0.00, 0.00},
    {0.00, 0.50, 0.00}
};
const double horizontal_half[3][3] = {
    {0.00, 0.00, 0.00},
    {0.50, 0.00, 0.50},
    {0.00, 0.00, 0.00}
};
const double diagonal_quarter[3][3] = {
    {0.25, 0.00, 0.25},
    {0.00, 0.00, 0.00},
    {0.25, 0.00, 0.25}
};
const double cross_quarter[3][3] = {
    {0.00, 0.25, 0.00},
    {0.25, 0.00, 0.25},
    {0.00, 0.25, 0.00}
};

//define a function to conduct various convolution
//template <size_t size_x1, size_t size_y1, size_t size_channels,size_t size_x2, size_t size_y2>
double convolution(unsigned char *img_padd,const double filter[3][3],int filter_size,int img_x,int img_y,int rows,int cols){
    int filter_x=0;
    int filter_y=0;
    double sum_val=0;  
    double mul_val=0;
	
    for (int i=img_x;i<img_x+filter_size;i++){
        for (int j=img_y;j<img_y+filter_size;j++){
            //mul_val=img_padd[i-1][j-1][0] * filter[filter_x][filter_y];
			mul_val=*((img_padd+i*cols+j)) * filter[filter_x][filter_y];
            filter_y+=1;
            sum_val=sum_val+mul_val;
        }
        filter_x+=1;
        filter_y=0;
    }
    return sum_val;
}


int main(int argc, char *argv[])

// the following code for read and write image file is taken from the source code provided
{
	// Define file pointer and variables
	FILE *file; 
	int BytesPerPixel;
	int Size_x =512 ;
	int Size_y =768;
	
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

    //code for convoultion with bilinear demosaicing filter
	unsigned char img_demosaiced [Size_x][Size_y][3];
	int flag_row=0;
	int flag_column=0;
	
	int filter_size=3;
	
	for (int i=1;i<Size_x+1;i++){
		flag_row=i%2;
		for (int j=1;j<Size_y+1;j++){
			flag_column=j%2;
			//cout<<"row"<<" "<<i<<","<<"column"<<" "<<j<<endl;
			double sum_valR=0;
			double sum_valG=0;
			double sum_valB=0;
			double mul_valR=0;
			double mul_valG=0;
			double mul_valB=0;
			int filter_x=0;
			int filter_y=0;
			if (flag_row==0 && flag_column==1){ //Blue index
				//cout<<"blue"<<endl;
				for (int conv_x=i;conv_x<i+filter_size;conv_x++){
					for (int conv_y=j;conv_y<j+filter_size;conv_y++){
						//cout<<"row"<<" "<<i<<","<<"column"<<" "<<j<<endl;
						mul_valR=img_padd[i-1+filter_x][j-1+filter_y][0]*diagonal_quarter[filter_x][filter_y];
						mul_valG=img_padd[i-1+filter_x][j-1+filter_y][0]*cross_quarter[filter_x][filter_y];
						sum_valR=sum_valR+mul_valR;
						sum_valG=sum_valG+mul_valG;
						filter_y=filter_y+1;
					}
					filter_x=filter_x+1;
					filter_y=0;
				}
				img_demosaiced[i-1][j-1][0]=sum_valR;
				img_demosaiced[i-1][j-1][1]=sum_valG;
				img_demosaiced[i-1][j-1][2]=Imagedata[i-1][j-1][0];
			}
			else if (flag_row==1 && flag_column==0){ //Red index
				//cout<<"red"<<endl;
				for (int conv_x=i;conv_x<i+filter_size;conv_x++){
					for (int conv_y=j;conv_y<j+filter_size;conv_y++){
						//cout<<"row"<<" "<<i<<","<<"column"<<" "<<j<<endl;
						mul_valG=img_padd[i-1+filter_x][j-1+filter_y][0]*cross_quarter[filter_x][filter_y];
						mul_valB=img_padd[i-1+filter_x][j-1+filter_y][0]*diagonal_quarter[filter_x][filter_y];
						sum_valG=sum_valG+mul_valG;
						sum_valB=sum_valB+mul_valB;
						filter_y=filter_y+1;
					}
					filter_x=filter_x+1;
					filter_y=0;
				}
				img_demosaiced[i-1][j-1][0]=Imagedata[i-1][j-1][0];
				img_demosaiced[i-1][j-1][1]=sum_valG;
				img_demosaiced[i-1][j-1][2]=sum_valB;
			}
			else if (flag_row==0 && flag_column==0){ //Green index case 1
				//cout<<"green-case1"<<endl;
				for (int conv_x=i;conv_x<i+filter_size;conv_x++){
					for (int conv_y=j;conv_y<j+filter_size;conv_y++){
						//cout<<"row"<<" "<<i<<","<<"column"<<" "<<j<<endl;
						mul_valR=img_padd[i-1+filter_x][j-1+filter_y][0]*vertical_half[filter_x][filter_y];
						mul_valB=img_padd[i-1+filter_x][j-1+filter_y][0]*horizontal_half[filter_x][filter_y];
						sum_valR=sum_valR+mul_valR;
						sum_valB=sum_valB+mul_valB;
						filter_y=filter_y+1;
					}
					filter_x=filter_x+1;
					filter_y=0;
				}
				img_demosaiced[i-1][j-1][0]=sum_valR;
				img_demosaiced[i-1][j-1][1]=Imagedata[i-1][j-1][0];
				img_demosaiced[i-1][j-1][2]=sum_valB;
			}
			else if (flag_row==1 && flag_column==1){ //Green index case 2
				//cout<<"green-case2"<<endl;
				for (int conv_x=i;conv_x<i+filter_size;conv_x++){
					for (int conv_y=j;conv_y<j+filter_size;conv_y++){
						//cout<<"row"<<" "<<i<<","<<"column"<<" "<<j<<endl;
						mul_valR=img_padd[i-1+filter_x][j-1+filter_y][0]*horizontal_half[filter_x][filter_y];
						mul_valB=img_padd[i-1+filter_x][j-1+filter_y][0]*vertical_half[filter_x][filter_y];
						sum_valR=sum_valR+mul_valR;
						sum_valB=sum_valB+mul_valB;
						filter_y=filter_y+1;
					}
					filter_x=filter_x+1;
					filter_y=0;
				}
				img_demosaiced[i-1][j-1][0]=sum_valR;
				img_demosaiced[i-1][j-1][1]=Imagedata[i-1][j-1][0];
				img_demosaiced[i-1][j-1][2]=sum_valB;
			}

		}
	}

    cout << "The input image is" <<endl;
	int i, j;
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
			cout<< (int)img_demosaiced[i][j][0]<<" "<< (int)img_demosaiced[i][j][1]<<" "<< (int)img_demosaiced[i][j][2]<<" "<<",";
		}
		cout<<endl;
    }

    // cout << "The input image is" <<endl;
	// for (i=246;i<256;i++){
	// 	for (j=246;j<256;j++){
	// 		cout<<(int) Imagedata[i][j][0]<<",";
	// 	}
	// 	cout<<endl;
    // }

	double result = convolution((unsigned char *)img_padd,horizontal_half,3,1,1,Size_x,Size_y);
	cout<<"result is "<<" "<<result;





	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(img_demosaiced, sizeof(unsigned char), Size_x*Size_y*3, file);
	fclose(file);

	return 0;
}

