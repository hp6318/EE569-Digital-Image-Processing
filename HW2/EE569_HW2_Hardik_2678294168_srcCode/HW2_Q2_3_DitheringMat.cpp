//EE559 ---  HW2_Q2a_3 ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio --- g++ compiler 
//Project: Dithering Matrix based Dithering
//Arguments to be passed: 4
    //1) Input image file(raw)
    //2) Output_d2x2 image file(raw)
    //3) Output_d8x8 image file(raw)
    //4) Output_d32x32 image file(raw)

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


void index_mat(int *mat_holder,int N,int *prev_mat){
    int row_idx=0;
    int col_idx=0;
    for (int i=0;i<N;i++){
        row_idx=i%(N/2);
        for (int j=0;j<N;j++){
            col_idx=j%(N/2);
            if(i<(N/2) && j<(N/2)){
               *(mat_holder+i*N+j)=*(prev_mat+row_idx*(N/2)+col_idx)*4+1; 
            }
            else if (i<(N/2) && j>=(N/2)){
                *(mat_holder+i*N+j)=*(prev_mat+row_idx*(N/2)+col_idx)*4+2;
            }
            else if (i>=(N/2) && j<(N/2)){
                *(mat_holder+i*N+j)=*(prev_mat+row_idx*(N/2)+col_idx)*4+3;
            }
            else {
                *(mat_holder+i*N+j)=*(prev_mat+row_idx*(N/2)+col_idx)*4+0;
            }
        }
    }
    
    //print out the Index matrix
    cout<<endl;
    cout<<"Index Matrix I_"<<N<<":"<<endl;
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            cout<<*(mat_holder+i*N+j)<<' ';
        }
        cout<<endl;
    }
}

void Dithering(unsigned char *input_img,unsigned char *output_img,int *index_mat,int *thr_mat,int size_x,int size_y,int N){
    int threshold;
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            *(thr_mat+i*N+j)=(int)(((*(index_mat+i*N+j)+0.5)/(N*N))*255);
        }
    }
    for (int i=0;i<size_x;i++){
        for (int j=0;j<size_y;j++){
            threshold=*(thr_mat+(i%N)*N+(j%N));
            if(*(input_img+i*size_y+j)<=threshold){
                *(output_img+i*size_y+j)=0;
            }
            else{
                *(output_img+i*size_y+j)=255;
            }
        }
    }
    //print out the Threshold matrix
    cout<<endl;
    cout<<"Threshold Matrix I_"<<N<<":"<<endl;
    for (int i=0;i<N;i++){
        for (int j=0;j<N;j++){
            cout<<*(thr_mat+i*N+j)<<' ';
        }
        cout<<endl;
    }
}

const int I_2[2][2]={
    {1,2},
    {3,0}
};

int main(int argc, char *argv[]){
    const int Size_x=400; //height
    const int Size_y=600; //width
    const int Bpp=1; //grayscale

    //Allocate dynamic memory for input image container
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);

    //Allocate dynamic memory for I_2 index mat container
    int *I2=new (nothrow) int [2*2];
    for (int i=0;i<2;i++){
        for (int j=0;j<2;j++){
            *(I2+i*2+j)=I_2[i][j];
        }
    }
    
    //Allocate dynamic memory for I4 index mat container
    int *I4=new (nothrow) int [4*4];
    index_mat(I4,4,I2);

    //Allocate dynamic memory for I8 index mat container
    int *I8=new (nothrow) int [8*8];
    index_mat(I8,8,I4);

    //deleting I4
    delete[] I4;

    //Allocate dynamic memory for I16 index mat container
    int *I16=new (nothrow) int [16*16];
    index_mat(I16,16,I8);

    //Allocate dynamic memory for I32 index mat container
    int *I32=new (nothrow) int [32*32];
    index_mat(I32,32,I16);

    //deleting I16
    delete[] I16;

    //Allocate dynamic memory for output_2 image container
    unsigned char *Output_2=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    //Allocate dynamic memory for Thr_2 index mat container
    int *Thr2=new (nothrow) int [2*2];
    //call Dithering function to generate output for I_2 
    Dithering(Imagedata,Output_2,I2,Thr2,Size_x,Size_y,2);
    //write the output_2 file 
    write(argv[2],Output_2,1,Size_x,Size_y);
    //deleting Thr2 and Output_2
    delete[] Thr2;
    delete[] Output_2;

    //Allocate dynamic memory for output_8 image container
    unsigned char *Output_8=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    //Allocate dynamic memory for Thr_8 index mat container
    int *Thr8=new (nothrow) int [8*8];
    //call Dithering function to generate output for I_8 
    Dithering(Imagedata,Output_8,I8,Thr8,Size_x,Size_y,8);
    //write the output_8 file 
    write(argv[3],Output_8,1,Size_x,Size_y);
    //deleting Thr8 and Output_8
    delete[] Thr8;
    delete[] Output_8;


    //Allocate dynamic memory for output_32 image container
    unsigned char *Output_32=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    //Allocate dynamic memory for Thr_32 index mat container
    int *Thr32=new (nothrow) int [32*32];
    //call Dithering function to generate output for I_32 
    Dithering(Imagedata,Output_32,I32,Thr32,Size_x,Size_y,32);
    //write the output_32 file 
    write(argv[4],Output_32,1,Size_x,Size_y);


    cout<<"complete"<<endl;
    return 0;
}
    