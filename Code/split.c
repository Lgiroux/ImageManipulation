#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "RasterImage.h"
#include "imageIO_TGA.h"

void splitColors(RasterImage* theImage, unsigned char* rRaster, unsigned char* gRaster, unsigned char* bRaster);
void fileNameExtract(char* fPath, char* fileName, int fileNameLen);
void newFileNames(char* outDir, char* fPath, char* fileName, char** files);


/**
 * @brief splits an input TGA image file into 3 different images - one consisting only of red
 * pixels, one of blue, and one of green
 * @param argc
 * @param argv takes name of image file to be split
 * @return
 */

int main(int argc, char** argv){


    //creates struct theImage using imageCreator within RASTER_IMAGE_H
    char* fPath = argv[1];
    RasterImage* theImage = imageCreator(fPath);

    //assume we have a nice user who is using correct input params

    char* outDir = argv[2];

    //to get the base of the file name from the user input

    int fileLen = strlen(fPath);
    int dirLen = 14; // '../Images/' + '.tga' = 14
    int fileNameLen = fileLen-dirLen;
    char* fileNameData;
    fileNameData = (char*)calloc(fileNameLen,sizeof(char));
    char** fileName = &fileNameData;


    fileNameExtract(fPath, *fileName, fileNameLen);

    //to create the 3 different names for each file
    char** filesList;
    filesList = (char**)calloc(3,sizeof(char*));
    char*** files = & filesList;


    newFileNames(outDir, fPath, *fileName, *files);


    unsigned char* rRasterData = (unsigned char*)calloc(4*theImage->numCols * theImage->numRows,sizeof(char));
    unsigned char* gRasterData = (unsigned char*)calloc(4*theImage->numCols * theImage->numRows,sizeof(char));
    unsigned char* bRasterData = (unsigned char*)calloc(4*theImage->numCols * theImage->numRows,sizeof(char));

    unsigned char** rRaster = &rRasterData;
    unsigned char** gRaster = &gRasterData;
    unsigned char** bRaster = &bRasterData;

    splitColors(theImage, *rRaster, *gRaster, *bRaster);


    int flag1 = writeTGA(filesList[0],rRasterData,theImage->numRows, theImage->numCols);
    if (flag1 == 1) printf("Error when writing to file\n");

    int flag2 = writeTGA(filesList[1],gRasterData,theImage->numRows, theImage->numCols);
    if (flag2 == 1) printf("Error 2 when writing to file\n");

    int flag3 = writeTGA(filesList[2],bRasterData,theImage->numRows, theImage->numCols);
    if (flag3 == 1) printf("Error 3 when writing to file\n");



}

/**
 * @brief splits theImage into 3 separate RasterImages, each consisting only of either
 * red, green, or blue values
 * @param theImage RasterImage to be split
 * @param rRaster RasterImage of theImage consisting only of red values
 * @param gRaster RasterImage of theImage consisting only of blue values
 * @param bRaster RasterImage of theImage consisting only of green values
 */
void splitColors(RasterImage* theImage, unsigned char* rRaster, unsigned char* gRaster, unsigned char* bRaster){

	int imgSize = (theImage->numCols * theImage->numRows);
	for (int i = 0; i < theImage->numRows; i++){
		for (int j = 0; j < theImage->numCols; j++){
			int k = 0;
			//set red values of green and blue rasters to 0
			//set red value of red raster to value of red pixel in raster
				rRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];
				bRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
				gRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
				//printf("red: %c green: %c blue: %c\n",rRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k], bRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k], gRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]);
			k = 1;//green values
				gRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];
				rRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
				bRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
			k = 2;//blue values
				bRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];
				rRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
				gRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=0;
			k = 3;//alpha values
				rRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];
				gRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];
				bRaster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k]=theImage->raster[i*theImage->bytesPerRow+j*theImage->bytesPerPixel+k];

			}
		}
}

/**
 * @brief extracts name of file from the file path
 * @param fPath entire path to image file
 * @param fileName name of file within the path
 * @param fileNameLen length of file name
 */
void fileNameExtract(char* fPath, char* fileName, int fileNameLen){

	// 10 = start of fileName after ../Images/)
	for(int i = 10; i < 10+fileNameLen; i++){
		fileName[i-10] = fPath[i];
	}
}

/**
 *
 * @param outDir directory to which RGB Images will be output
 * @param fPath path to input image
 * @param fileName name of image without .tga
 * @param files list of names of files to be used for output
 */
void newFileNames(char* outDir, char* fPath, char*fileName, char** files){

	char endOfPath[] = ".tga";

	int fileLen = strlen(fPath);

	int x = strlen(outDir);
	int y =  strlen(fileName);

	for(int i = 0; i < 3; i++){
		files[i] = (char*)calloc((fileLen+3),sizeof(char));
	}


	char r[] = {'_','r'};
	char g[] = {'_','g'};
	char b[] = {'_','b'};




//3 times for 3 rasters
	strcat(files[0],outDir);
	strcat(files[0],fileName);
	strcat(files[0],r);
	strcat(files[0],endOfPath);

	strcat(files[1],outDir);
	strcat(files[1],fileName);
	strcat(files[1],g);
	strcat(files[1],endOfPath);

	strcat(files[2],outDir);
	strcat(files[2],fileName);
	strcat(files[2],b);
	strcat(files[2],endOfPath);
}
