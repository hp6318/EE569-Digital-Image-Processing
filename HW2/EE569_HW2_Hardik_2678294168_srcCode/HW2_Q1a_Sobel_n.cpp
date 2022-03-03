//EE559 ---  HW2_Q1A ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio --- g++ compiler 
//Project: Sobel edge detection
//Arguments to be passed: 5
    //1) Input image file(raw)
    //2) Gradient_x output file(raw)
    //3) Gradient_y output file(raw)
    //4) Gradient Magnitude output file(raw)
    //5) Sobel binary edge map(raw) 
//Location to excel file to write CDF values(gradient magnitude) is hard-coded at Line 221

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <string.h>
#include <fstream>
using namespace std;

//sobel_x filter
const float sobel_x[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}
};

//sobel_y filter
const float sobel_y[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}
};

void read(const char *filename,void *image_mat, int bpp, int size_x, int size_y){
    FILE *file;
    if (!(file=fopen(filename,"rb"))) {
		cout << "Cannot open file: " << filename <<endl;
		exit(1);
	}
	fread(image_mat, sizeof(unsigned char), size_x*size_y*bpp, file);
	fclose(file);
}

void write(const char *filename,void *image_mat, int bpp, int size_x, int size_y){
    FILE *file;
    if (!(file=fopen(filename,"wb"))) {
		cout << "Cannot open file: " << filename << endl;
		exit(1);
	}
	fwrite(image_mat, sizeof(unsigned char), size_x*size_y*bpp, file);
	fclose(file);
}

int clip(float value){
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

float convolution(unsigned char *img_padd,const float filter[3][3],int filter_size,int img_x,int img_y,int rows,int cols){
    int filter_x=0;
    int filter_y=0;
    float sum_val=0;  
    float mul_val=0;
	
    for (int i=img_x;i<img_x+filter_size;i++){
        for (int j=img_y;j<img_y+filter_size;j++){
			mul_val=*((img_padd+i*cols+j)) * filter[filter_x][filter_y];
            filter_y+=1;
            sum_val=sum_val+mul_val;
        }
        filter_x+=1;
        filter_y=0;
    }
    //sum_val=sum_val/16;
    return sum_val;
}

void normalize(float *grad_xy,double max, double min,unsigned char *grad_out,int size_x,int size_y){
    for (int i=0; i<size_x; i++){
        for (int j=0; j<size_y; j++){
            *(grad_out+i*size_y+j)=(255*(*(grad_xy+i*size_y+j)-min))/(max-min);
        }
    }
}


int main(int argc, char *argv[]){
    const int Size_x=321; //height
    const int Size_y=481; //width
    const int Bpp=3; //rgb

    //Allocate dynamic memory for input image container
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);

    cout<<"The image Matrix is "<<endl;
    //values are stored RGB, RGB, RGB. Imagedata points to 0th index which is 'R' 
    //value of 0th row & 0th column
    for (int i=0;i<5;i++){
        cout<<(int)*(Imagedata+3*i)<<" "<<(int)*(Imagedata+3*i+1)<<" "<<(int)*(Imagedata+3*i+2)<<","<<" ";
    }
    cout<<endl;

    //Allocate dynamic memory for grayscale image container
    unsigned char *Imagedata_gray=new (nothrow) unsigned char [Size_x*Size_y*1];
    //extract the luminance channel, Y channel.
    for (int i=0;i<Size_x*Size_y;i++){
        *(Imagedata_gray+i)=*(Imagedata+3*i)*0.2989 + *(Imagedata+3*i+1)*0.5870 + *(Imagedata+3*i+2)*0.1140 ;
    }
    
    //delete the memory of input rgb image
     delete[] Imagedata;

    //allocate memory for padded image 
    const int padd_size_x=Size_x+2;
	const int padd_size_y=Size_y+2;
    unsigned char img_padd[padd_size_x][padd_size_y][1];
    
    //copying the center core 
    for (int i=0;i<padd_size_x-2;i++){
        for (int j=0;j<padd_size_y-2;j++){
            img_padd[i+1][j+1][0]=*(Imagedata_gray+i*Size_y+j);
        }
    }
    //adding reflected column on left
    for (int i=0;i<padd_size_x-2;i++){
        img_padd[i+1][0][0]=*(Imagedata_gray+1+i*Size_y);
    }
    //adding reflected column on right
    for (int i=0;i<padd_size_x-2;i++){
        img_padd[i+1][padd_size_y-1][0]=*(Imagedata_gray+(Size_y-2)+i*Size_y);
    }
    //adding reflected row on top and bottom
    for (int i=0;i<padd_size_y;i++){
        img_padd[0][i][0]=img_padd[2][i][0];
        img_padd[padd_size_x-1][i][0]=img_padd[padd_size_x-3][i][0];
    }

    //compute x_gradient and y_gradient by convolving with sobel filter and padded image
    float gradient_x [Size_x][Size_y];
    float gradient_y [Size_x][Size_y];
	int filter_size=3;
	double xgrad_max=0;
    double xgrad_min=0;
	double ygrad_max=0;
    double ygrad_min=0;

	for (int i=1;i<Size_x+1;i++){
		for (int j=1;j<Size_y+1;j++){
            float result = convolution((unsigned char *)img_padd,sobel_x,3,i-1,j-1,padd_size_x,padd_size_y);
            if (result>xgrad_max){
                xgrad_max=result;
            }
            if (result<xgrad_min){
                xgrad_min=result;
            }
            gradient_x[i-1][j-1]=floor(result);
            result = convolution((unsigned char *)img_padd,sobel_y,3,i-1,j-1,padd_size_x,padd_size_y);
            if (result>ygrad_max){
                ygrad_max=result;
            }
            if (result<ygrad_min){
                ygrad_min=result;
            }
            gradient_y[i-1][j-1]=floor(result);
        }
    }


    //normalize the gradient x and y to 0-255 in order to visulaize. 
    //Allocate dynamic memory for gradient_x image container
    unsigned char *grad_x=new (nothrow) unsigned char [Size_x*Size_y];
    normalize(*gradient_x,xgrad_max, xgrad_min,grad_x,Size_x,Size_y);
    write(argv[2],grad_x,1,Size_x,Size_y);
    delete[] grad_x;

    //Allocate dynamic memory for gradient_y image container
    unsigned char *grad_y=new (nothrow) unsigned char [Size_x*Size_y];
    normalize(*gradient_y,ygrad_max, ygrad_min,grad_y,Size_x,Size_y);
    write(argv[3],grad_y,1,Size_x,Size_y);
    delete[] grad_y;

    //compute gradient magnitude= sqrt (grad_x^2 + grad_y^2)
    float grad_mag[Size_x][Size_y];
    double grad_max=0;
    double grad_min=sqrt(pow(gradient_x[0][0],2)+pow(gradient_y[0][0],2));
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            grad_mag[i][j]=sqrt(pow(gradient_x[i][j],2)+pow(gradient_y[i][j],2));
            if (grad_mag[i][j]>grad_max){
                grad_max=grad_mag[i][j];
            }
            if (grad_mag[i][j]<grad_min){
                xgrad_min=grad_mag[i][j];
            }
        }
    }
    //normalize the gradient magnitude to 0-255 in order to visulaize. 
    //Allocate dynamic memory for gradient_magnitude image container
    unsigned char *gradient_mag=new (nothrow) unsigned char [Size_x*Size_y];
    normalize(*grad_mag,grad_max, grad_min,gradient_mag,Size_x,Size_y);
    write(argv[4],gradient_mag,1,Size_x,Size_y);
    delete[] gradient_mag;

    //compute pdf of gradient magnitude by dividing by 255 and clipping it to 0-255.
    double histogram[256]={0}; //0-255 intensity values
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            histogram[clip(grad_mag[i][j])]=histogram[clip(grad_mag[i][j])]+1;
        }
    }

    int total_px=Size_x*Size_y;
    double pdf[256]={0}; 
    cout<<"PDF"<<endl;
    for (int i=0;i<256;i++){
        pdf[i]=histogram[i]/total_px;
        //cout<<i<<"-"<<pdf[i]<<endl;
    }

    //compute cdf by cummulative addition of pdf with increasing value of gradient magnitude
    double cdf[256]={0};
    cdf[0]=pdf[0];
    cout<<"CDF"<<endl;
    for (int i=1;i<256;i++){
        cdf[i]=cdf[i-1]+pdf[i];
        //cout<<i<<"-"<<cdf[i]<<endl;
    }    
    //exporting the cdf values to excel for plotting
    fstream fout;
    fout.open("E:\\usc\\Spring'22\\EE569 Image processing\\HW2\\results_final\\tiger_gradMag_cdf_f.csv", ios::out | ios::app);
    for (int i=0;i<256;i++){
        fout<<i<<","<<cdf[i]<<"\n";
    }
    fout.close();

    //find the threshold value by capping the cdf at defined %
    float thres_per =0.95;
    int threshold=0;
    while (threshold<256){
        if(cdf[threshold]>thres_per){
            break;
        }
        threshold++;
    }
    threshold=threshold-1;
    cout<<"threshold is "<<threshold<<endl;

    //assign memory for output binary image container
    unsigned char *output=new (nothrow) unsigned char [Size_x*Size_y];
    //create edge map by comparing gradient magnitude values with threshold.
    for (int i=0;i<Size_x;i++){
        for (int j=0;j<Size_y;j++){
            if (grad_mag[i][j]>=threshold){
                *(output+i*Size_y+j)=0; // black edge 
            }
            else{
                *(output+i*Size_y+j)=255;  //white background
            }
        }
    }
    write(argv[5],output,1,Size_x,Size_y);



    // cout << "The input image is" <<endl;
	// for (int i=0;i<5;i++){
	// 	for (int j=0;j<5;j++){
	// 		cout<< (int)*(Imagedata_gray+i*Size_y+j)<<",";
	// 	}
	// 	cout<<endl;
    // }


    // cout << "The padded image is" <<endl;
	// for (int i=0;i<5;i++){
	// 	for (int j=0;j<5;j++){
	// 		cout<< (int)img_padd[i][j][0]<<",";
	// 	}
	// 	cout<<endl;
    // }

    // cout << "The gradient_x is" <<endl;
	// for (int i=0;i<5;i++){
	// 	for (int j=0;j<5;j++){
	// 		cout<< (int)gradient_x[i][j][0]<<",";
	// 	}
	// 	cout<<endl;
    // }


    // cout << "The gradient_y is" <<endl;
	// for (int i=0;i<5;i++){
	// 	for (int j=0;j<5;j++){
	// 		cout<< (int)gradient_y[i][j][0]<<",";
	// 	}
	// 	cout<<endl;
    // }



    cout<<"complete"<<endl;
    return 0;
}
    