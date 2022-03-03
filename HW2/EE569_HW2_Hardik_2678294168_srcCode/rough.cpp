#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#define  _CRT_SECURE_NO_WARNINGS 
#include <string.h>

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
    const int Size_x=321;
    const int Size_y=481;
    const int Bpp=3;

    //Allocate dynamic memory for input image container
    unsigned char *Imagedata=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[1],Imagedata,Bpp,Size_x,Size_y);

    cout<<"The image Matrix is "<<endl;
    for (int i=0;i<5;i++){
        cout<<(int)*(Imagedata+3*i)<<" "<<(int)*(Imagedata+3*i+1)<<" "<<(int)*(Imagedata+3*i+2)<<","<<" ";
    }
    cout<<endl;
    
    // unsigned char Imagedata[Size_x][Size_y][Bpp];
    // read(argv[1],Imagedata,Bpp,Size_x,Size_y);
    // cout<<"The image Matrix is "<<endl;
    // for (int i=0;i<5;i++){
    //     for (int j=0;j<5;j++){
    //         cout<<(int)Imagedata[i][j][0]<<" "<<(int)Imagedata[i][j][1]<<" "<<(int)Imagedata[i][j][2]<<","<<" ";
    //     }
    //     cout<<endl;
    // }

    delete[] Imagedata;
    cout<<"The image Matrix after first deletion is "<<endl;
    
    //Allocate dynamic memory for input image container
    unsigned char *Imagedata1=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[2],Imagedata1,Bpp,Size_x,Size_y);

    cout<<"The 2nd image Matrix is "<<endl;
    for (int i=0;i<5;i++){
        cout<<(int)*(Imagedata1+3*i)<<" "<<(int)*(Imagedata1+3*i+1)<<" "<<(int)*(Imagedata1+3*i+2)<<","<<" ";
    }
    cout<<endl;

    delete[] Imagedata1; 
    
    cout<<"The image Matrix after deletion 2 is "<<endl;
    
    //Allocate dynamic memory for input image container
    unsigned char *Imagedata2=new (nothrow) unsigned char [Size_x*Size_y*Bpp];
    read(argv[3],Imagedata2,Bpp,Size_x,Size_y);

    cout<<"The 3rd image Matrix is "<<endl;
    for (int i=0;i<5;i++){
        cout<<(int)*(Imagedata2+3*i)<<" "<<(int)*(Imagedata2+3*i+1)<<" "<<(int)*(Imagedata2+3*i+2)<<","<<" ";
    }
    cout<<endl;
    
    write(argv[4],Imagedata2,Bpp,Size_x,Size_y);
    
    return 0;
}
