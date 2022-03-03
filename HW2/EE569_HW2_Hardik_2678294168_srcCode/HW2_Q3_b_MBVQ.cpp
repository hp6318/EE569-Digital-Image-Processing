//EE559 ---  HW3_b ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio --- g++ compiler 
//Project: Color-Half Toning with MVBQ based Error Diffusion(FS)
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


void error_defusion(double *F_hat,double error,int idx_r,int idx_c,double *h_mat,int Size_filter,int size_x,int size_y,double divisor){
    int filter_x=0;
    int filter_y=0;
    for (int i=idx_r-((Size_filter-1)/2);i<idx_r-((Size_filter-1)/2)+Size_filter;i++){
        for (int j=idx_c-((Size_filter-1)/2);j<idx_c-((Size_filter-1)/2)+Size_filter;j++){
            *(F_hat+i*size_y+j)=*(F_hat+i*size_y+j) + (error*(*(h_mat+filter_x*Size_filter+filter_y)))/divisor;
            filter_y=filter_y+1; 
        }
        filter_y=0;
        filter_x=filter_x+1;
    }
}

void nearest_vertex(int quadrant,double *f_hat_r,double *f_hat_g,double *f_hat_b,unsigned char *output_img,int idx,int cnt){
    //CMYW-1, MYGC-2, RGMY-3, KRGB-4, RGBM-5, CMGB-6
    string vertex;
    double normalize=255;
    double R=*(f_hat_r+cnt)/normalize;
    double G=*(f_hat_g+cnt)/normalize;
    double B=*(f_hat_b+cnt)/normalize;
    //CMYW
    if (quadrant == 1){
        vertex = "white";
        if (B < 0.5){
            if (B <= R){
                if (B <= G){
                    vertex = "yellow";
                }
            }
        }
        if (G < 0.5){
            if (G <= B){
                if (G <= R){
                    vertex = "magenta";
                }
            }
        }
        if (R < 0.5){
            if (R <= B){
                if (R <= G){
                    vertex = "cyan";
                }
            }
        }
    }


    // No.2 for MYGC
    if (quadrant == 2){
        vertex = "magenta"; 
        if (G >= B){
            if (R >= B){
                if (R >= 0.5){
                    vertex = "yellow";
                }
                else{
                    vertex = "green";
                }
            }
        }
        if (G >= R){
            if (B >= R){
                if (B >= 0.5){
                    vertex = "cyan"; 
                }
                else{
                    vertex = "green";
                }
            }
        }
    }


    // No.3 for RGMY
    if (quadrant == 3){
        if (B > 0.5){
            if (R > 0.5){
                if (B >= G){
                    vertex = "magenta";
                }
                else{
                    vertex = "yellow";
                }
            }
            else{
                if (G > B + R){
                    vertex = "green";
                }
                else{ 
                    vertex = "magenta";
                }
            }
        }
        else{
            if (R >= 0.5){
                if (G >= 0.5){
                    vertex = "yellow";
                }
                else{
                    vertex = "red";
                }
            }
            else{
                if (R >= G){
                    vertex = "red";
                }
                else{
                    vertex = "green";
                }
            }
        }
    }


    //No.4 for KRGB
    if (quadrant == 4){
        vertex = "black";
        if (B > 0.5){
            if (B >= R){
                if (B >= G){
                    vertex = "blue";
                }
            }
        }
        if (G > 0.5){
            if (G >= B){
                if (G >= R){
                    vertex = "green";
                }
            }
        }
        if (R > 0.5){
            if (R >= B){
                if (R >= G){
                    vertex = "red";
                }
            }
        }
    }


    // No.5 for RGBM
    if (quadrant == 5){
        vertex = "green";
        if (R > G){
            if (R >= B){
                if (B < 0.5){
                    vertex = "red";
                }
                else{
                    vertex = "magenta";
                }
            }
        }
        if (B > G){
            if (B >= R){
                if (R < 0.5){
                    vertex = "blue";
                }
                else{
                    vertex = "magenta";
                }
            }
        }
    }


    // No.6 for CMGB
    if (quadrant == 6){
        if (B > 0.5){
            if ( R > 0.5){
                if (G >= R){
                    vertex = "cyan";
                }
                else{
                    vertex = "magenta";
                }
            }
            else{
                if (G > 0.5){
                    vertex = "cyan";
                }
                else{
                    vertex = "blue";
                }
            }
        }
        else{
            if ( R > 0.5){
                if (R - G + B >= 0.5){
                    vertex = "magenta";
                }
                else{
                    vertex = "green";
                }
            }
            else{
                if (G >= B){
                    vertex = "green";
                }
                else{
                    vertex = "blue";
                }
            }
        }
    }
    if (vertex=="red"){
        *(output_img+idx)=255;
        *(output_img+idx+1)=0;
        *(output_img+idx+2)=0;
    }
    else if (vertex=="green"){
        *(output_img+idx)=0;
        *(output_img+idx+1)=255;
        *(output_img+idx+2)=0;
    }
    else if (vertex=="blue"){
        *(output_img+idx)=0;
        *(output_img+idx+1)=0;
        *(output_img+idx+2)=255;
    }
    else if (vertex=="cyan"){
        *(output_img+idx)=0;
        *(output_img+idx+1)=255;
        *(output_img+idx+2)=255;
    }
    else if (vertex=="magenta"){
        *(output_img+idx)=255;
        *(output_img+idx+1)=0;
        *(output_img+idx+2)=255;
    }
    else if (vertex=="yellow"){
        *(output_img+idx)=255;
        *(output_img+idx+1)=255;
        *(output_img+idx+2)=0;
    }
    else if (vertex=="white"){
        *(output_img+idx)=255;
        *(output_img+idx+1)=255;
        *(output_img+idx+2)=255;
    }
}

int MBVQ_quadruple(unsigned char *original_image, int idx){
    int output=0; //CMYW-1, MYGC-2, RGMY-3, KRGB-4, RGBM-5, CMGB-6
    if (*(original_image+idx)+*(original_image+idx+1)>255){
        if(*(original_image+idx+1)+*(original_image+idx+2)>255){
            if(*(original_image+idx)+*(original_image+idx+1)+*(original_image+idx+2)>510){
                output=1; //CMYW
            }
            else{
                output=2; //MYGC
            }
        }
        else{
            output=3; //RGMY
        }
    }
    else{
        if (!((*(original_image+idx+1)+*(original_image+idx+2))>255)){
            if(!((*(original_image+idx)+*(original_image+idx+1)+*(original_image+idx+2))>255)){
                output=4; //KRGB
            }
            else{
                output=5; //RGBM
            }
        }
        else{
            output=6; //CMGB
        }
    }
    return output;
}

void channel_sep(double *f_hat,unsigned char *input_image,int size_x,int size_y,int channel){
    int grayscale_idx=0;
    for (int i=0+channel;i<size_x*size_y*3;i++){
        *(f_hat+grayscale_idx)=*(input_image+i);
        grayscale_idx=grayscale_idx+1;
        i=i+2;
    }
}

void boundary_px(unsigned char *input_img,double *f_hat_r,double *f_hat_g,double *f_hat_b,unsigned char *output_img,int rows,int cols){
    //first row
    int quad=0;
    int cnt=0;
    for (int i=0;i<cols*3;i++){
        quad=MBVQ_quadruple(input_img,i);
        nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,i,cnt);
        i=i+2;
        cnt=cnt+1;
    }

    //first col
    cnt=0;
    for (int i=cols*3;i<rows*cols*3;i++){
        quad=MBVQ_quadruple(input_img,i);
        nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,i,cnt);
        i=(i-1)+3*cols;
        cnt=cnt+1;
    }

    //last col
    cnt=0;
    for (int i=(cols-1)*3;i<rows*cols*3;i++){
        quad=MBVQ_quadruple(input_img,i);
        nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,i,cnt);
        i=(i-1)+3*cols;
        cnt=cnt+1;
    }

    //last row
    cnt=0;
    for (int i=(rows-1)*(cols*3)+1;i<rows*cols*3-3;i++){
        quad=MBVQ_quadruple(input_img,i);
        nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,i,cnt);
        i=i+2;
        cnt=cnt+1;
    }
}

void MBVQ_experiment(unsigned char *input_img, unsigned char *output_img,double *h_mat,double *h_mat_r,double *f_hat_r, double *f_hat_g,double *f_hat_b,int size_x, int size_y, int filter_size,double divisor){
    //separate rgb channel
    channel_sep(f_hat_r,input_img,size_x,size_y,0);
    channel_sep(f_hat_g,input_img,size_x,size_y,1);
    channel_sep(f_hat_b,input_img,size_x,size_y,2);


    //quantize the boundary pixels
    boundary_px(input_img,f_hat_r,f_hat_g,f_hat_b,output_img,size_x,size_y);

    //iterate over each pixel and do serpentine scanning
    int cnt=0;
    int idx=0;
    int quad=0;
    double err_r=0;
    double err_g=0;
    double err_b=0;
    for (int i=(filter_size-1)/2;i<size_x-((filter_size-1)/2);i++){
        if ((i%2==1 && (((filter_size-1)/2))%2==1) || (i%2==0 && (((filter_size-1)/2))%2==0)){ //left to right
            for (int j=(filter_size-1)/2;j<size_y-((filter_size-1)/2);j++){
                cnt=i*size_y+j;
                idx=cnt*3;
                //find the quadruple based on RGB value of input image
                quad=MBVQ_quadruple(input_img,idx);
                //find nearest vertex based on updated RGB value: RGB+error
                nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,idx,cnt);
                err_r=*(f_hat_r+cnt)-(double)*(output_img+idx);
                err_g=*(f_hat_g+cnt)-(double)*(output_img+idx+1);
                err_b=*(f_hat_b+cnt)-(double)*(output_img+idx+2);
                //diffuse the error to future pixels
                error_defusion(f_hat_r,err_r,i,j,h_mat,filter_size,size_x,size_y,divisor);
                error_defusion(f_hat_g,err_g,i,j,h_mat,filter_size,size_x,size_y,divisor);
                error_defusion(f_hat_b,err_b,i,j,h_mat,filter_size,size_x,size_y,divisor);
            }
        }
        else{ //right to left
            for(int j=size_y-((filter_size-1)/2)-1;j>=(filter_size-1)/2;j--){
                cnt=i*size_y+j;
                idx=cnt*3;
                //find the quadruple based on RGB value of input image
                quad=MBVQ_quadruple(input_img,idx);
                //find nearest vertex based on updated RGB value: RGB+error
                nearest_vertex(quad,f_hat_r,f_hat_g,f_hat_b,output_img,idx,cnt);
                err_r=*(f_hat_r+cnt)-(double)*(output_img+idx);
                err_g=*(f_hat_g+cnt)-(double)*(output_img+idx+1);
                err_b=*(f_hat_b+cnt)-(double)*(output_img+idx+2);
                //diffuse the error to future pixels
                error_defusion(f_hat_r,err_r,i,j,h_mat_r,filter_size,size_x,size_y,divisor);
                error_defusion(f_hat_g,err_g,i,j,h_mat_r,filter_size,size_x,size_y,divisor);
                error_defusion(f_hat_b,err_b,i,j,h_mat_r,filter_size,size_x,size_y,divisor);
            }
        }
    }
}

int main(int argc, char *argv[]){
    const int Size_x=375; //height
    const int Size_y=500; //width
    const int Bpp=3; //rgb

    //Allocate dynamic memory for input image container & initialize f_hat=input image
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);

    //Allocate dynamic memory for output image container 
    unsigned char *Output_rgb=new (nothrow) unsigned char [Size_x*Size_y*Bpp];

    //Allocate dynamic memory for f_hat and copy input image to it changing datatype to double (channel wise)
    double *f_hat_r=new (nothrow) double [Size_x*Size_y];
    double *f_hat_g=new (nothrow) double [Size_x*Size_y];
    double *f_hat_b=new (nothrow) double [Size_x*Size_y];
    
    //run the experiment
    double divisor=16;
    MBVQ_experiment(Imagedata,Output_rgb,*floyd,*floyd_ref,f_hat_r,f_hat_g,f_hat_b,Size_x,Size_y,3,divisor);
    
    
    //write the final output rgb image of the experiment
    write(argv[2],Output_rgb,3,Size_x,Size_y);
    //cout<<divisor-(double)*(Output_rgb+4506)<<" "<<(double)*(Output_rgb+4507)<<" "<<(double)*(Output_rgb+4508)<<endl;
    cout<<"complete"<<endl;
    return 0;
}
    