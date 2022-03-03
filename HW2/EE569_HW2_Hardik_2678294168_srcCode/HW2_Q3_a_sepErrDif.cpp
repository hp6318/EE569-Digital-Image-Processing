//EE559 ---  HW3_a ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio --- g++ compiler 
//Project: Color-Half Toning with separable Error Diffusion
//Arguments to be passed: 2
    //1) Input image file(raw)
    //2) Output image file(raw)

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <string.h>
#include <fstream>
using namespace std;


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

double floyd[3][3]={
    {0,0,0},
    {0,0,7},
    {3,5,1}
};

double floyd_ref[3][3]={
    {0,0,0},
    {7,0,0},
    {1,5,3}
};

void binarize(unsigned char *Binarized_mat,double *F_hat_mat,int size_x,int size_y,int threshold){
    for (int i=0;i<size_x;i++){
        for (int j=0;j<size_y;j++){
            if(*(F_hat_mat+i*size_y+j)>threshold){
                *(Binarized_mat+i*size_y+j)=255;
            }
            else{
                *(Binarized_mat+i*size_y+j)=0;
            }
        }
    }
}

int bin_flag(double pixel,int threshold){
    int flag=0;
    if (pixel>threshold){
        flag=255;
    }
    else{
        flag=0;
    }
    return flag;
}

void error_defusion(double *F_hat,double error,int idx_r,int idx_c,double *h_mat,int Size_filter,int size_x,int size_y,double divisor){
    int filter_x=0;
    int filter_y=0;
    //if ((idx_r%2==1 && (((Size_filter-1)/2))%2==1) || (idx_r%2==0 && (((Size_filter-1)/2))%2==0)){ //left to right
    for (int i=idx_r-((Size_filter-1)/2);i<idx_r-((Size_filter-1)/2)+Size_filter;i++){
        for (int j=idx_c-((Size_filter-1)/2);j<idx_c-((Size_filter-1)/2)+Size_filter;j++){
            *(F_hat+i*size_y+j)=*(F_hat+i*size_y+j) + (error*(*(h_mat+filter_x*Size_filter+filter_y)))/divisor;
            filter_y=filter_y+1; 
        }
        filter_y=0;
        filter_x=filter_x+1;
    }
}

void experiment(double *f_hat, unsigned char *binary_mat,double *h_mat,double *h_mat_mirror,int size_x,int size_y,int filter_size,int threshold_val,double divisor){
    double error=0;
    int b_pxl=0;
    for (int i=(filter_size-1)/2;i<size_x-((filter_size-1)/2);i++){
        if ((i%2==1 && (((filter_size-1)/2))%2==1) || (i%2==0 && (((filter_size-1)/2))%2==0)){ //left to right
            for (int j=(filter_size-1)/2;j<size_y-((filter_size-1)/2);j++){
                //binarize(binary_mat,f_hat,size_x,size_y,threshold_val);
                b_pxl=bin_flag(*(f_hat+i*size_y+j),threshold_val);
                error=*(f_hat+i*size_y+j) - (double)b_pxl;
                error_defusion(f_hat,error,i,j,h_mat,filter_size,size_x,size_y,divisor);
            }
        }
        else{ //right to left
            for(int j=size_y-((filter_size-1)/2);j>=(filter_size-1)/2;j--){
                //binarize(binary_mat,f_hat,size_x,size_y,threshold_val);
                b_pxl=bin_flag(*(f_hat+i*size_y+j),threshold_val);
                error=*(f_hat+i*size_y+j) - (double)b_pxl;
                //error=*(f_hat+i*size_y+j) - (*(binary_mat+i*size_y+j));
                error_defusion(f_hat,error,i,j,h_mat_mirror,filter_size,size_x,size_y,divisor);
            }
        }
    }
    binarize(binary_mat,f_hat,size_x,size_y,threshold_val); //final binarization
}

void channel_sep(double *f_hat,unsigned char *input_image,int size_x,int size_y,int channel){
    int grayscale_idx=0;
    for (int i=0+channel;i<size_x*size_y*3;i++){
        *(f_hat+grayscale_idx)=255.0-*(input_image+i);
        grayscale_idx=grayscale_idx+1;
        i=i+2;
    }
}

void channel_stack(unsigned char *output_channel,unsigned char *output_image,int size_x,int size_y,int channel){
    int grayscale_idx=0;
    for (int i=0+channel;i<size_x*size_y*3;i++){
        *(output_image+i)=255.0-*(output_channel+grayscale_idx);
        grayscale_idx=grayscale_idx+1;
        i=i+2;
    }
}

int main(int argc, char *argv[]){
    const int Size_x=375; //height
    const int Size_y=500; //width
    const int Bpp=3; //rgb

    //Allocate dynamic memory for input image container & initialize f_hat=input image
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);

    //Allocate dynamic memory for ooutput image container 
    unsigned char *Output_rgb=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    
    //Allocate dynamic memory for channel wise output container
    unsigned char *output_chn=new (nothrow) unsigned char [Size_x*Size_y];

    //Allocate dynamic memory for f_hat 
    double *f_hat=new (nothrow) double [Size_x*Size_y];
    
    double divisor=16; 

    for (int i=0;i<3;i++){
        //separate the channel, convert to CMY space & initialize f_hat=255-input image
        channel_sep(f_hat,Imagedata,Size_x,Size_y,i); 
        //run the experiment channel wise
        experiment(f_hat,output_chn,*floyd,*floyd_ref,Size_x,Size_y,3,128,divisor);
        //copy the binarized matrix (channel wise) to the final output image.
        channel_stack(output_chn,Output_rgb, Size_x,Size_y,i);
    }
    //write the final output rgb image of the experiment
    write(argv[2],Output_rgb,3,Size_x,Size_y);

    cout<<"complete"<<endl;
    return 0;
}
    