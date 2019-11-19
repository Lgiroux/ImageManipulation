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


/**
 *
 * @param argc number of arguments passed
 * @param argv dimensions is called with the path to a TGA image file
 * preceded by either nothing, -h, -w, -v, or -v with -h or -w
 * -v implies a "verbose" output of the image dimensions
 * @return prints the width and/or height, in pixels, of the file passed as input
 */

int main(int argc, char** argv){


	//creates struct theImage using imageCreator within RASTER_IMAGE_H
	char* fPath = argv[argc-1];
	RasterImage* theImage = imageCreator(fPath);


	//*******************************
	//tests to check for valid inputs
	//*******************************

	if(argc == 2) printf("%d %d\n",theImage->numCols,theImage->numRows);
	else if(argc == 3){
		if(strcmp(argv[1],"-h")==0) printf("%d\n", theImage->numRows);
		else if(strcmp(argv[1],"-w")==0)printf("%d\n", theImage->numCols);
		else if(strcmp(argv[1],"-v")==0) printf("width: %d, height: %d\n",theImage->numCols,theImage->numRows);
		else printf("Invalid modifiers\n");
	}

	else if(argc == 4){
		if((strcmp(argv[1],"-v")==0)){
			if((strcmp(argv[1],argv[2])!=0) && (strcmp(argv[2],"-h")==0)) printf("height: %d\n",theImage->numRows);
			else if((strcmp(argv[1],argv[2])!=0) && (strcmp(argv[2],"-w")==0)) printf("width: %d\n",theImage->numCols);
			else printf("Invalid modifiers\n");
		}
		else if ((strcmp(argv[1],"-h")==0) && (strcmp(argv[2],"-v")==0)) printf("height: %d\n",theImage->numRows);
		else if ((strcmp(argv[1],"-w")==0) && (strcmp(argv[2],"-v")==0)) printf("width: %d\n",theImage->numCols);
		else printf("Invalid modifiers\n");
		}




return 0;
}
