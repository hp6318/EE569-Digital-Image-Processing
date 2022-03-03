//EE559 ---  HW2_Q2a_2 ---  Hardik Prajapati --- 2678294168 
//Windows --- Visual Studio --- g++ compiler 
//Project: Random Threshold based Dithering
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



int main(int argc, char *argv[]){
    const int Size_x=400; //height
    const int Size_y=600; //width
    const int Bpp=1; //grayscale

    //Allocate dynamic memory for input image container
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);
    
    //Initialize the random generator 
    srand(time(0));
    int threshold=0;

    //Allocate dynamic memory for output image container
    unsigned char *output=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    for (int i=0;i<Size_x*Size_y;i++){
        threshold=rand()%255;
        if (*(Imagedata+i)<threshold){
            *(output+i)=0;
        }
        else{
            *(output+i)=255;
        }
    }

    write(argv[2],output,1,Size_x,Size_y);


    cout<<"complete"<<endl;
    return 0;
}
    