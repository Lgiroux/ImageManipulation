#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "RasterImage.h"
#include "imageIO_TGA.h"

RasterImage* imageCreator(char* fPath){
	//allocate onto heap so instance stays after function call
	RasterImage *theImage = (RasterImage*)calloc(1,sizeof(RasterImage));

	//read data of image into RasterImage
	theImage->raster = readTGA(fPath, &theImage->numRows, &theImage->numCols, &theImage->theType);

	//determine type of image
	if (theImage->raster != NULL)
	{
		switch (theImage->theType)
		{
			case RGBA32_RASTER:
			case FLOAT_RASTER:
				theImage->bytesPerPixel = 4;
				break;

			case GRAY_RASTER:
				theImage->bytesPerPixel = 1;
			break;
		}

		theImage->bytesPerRow = theImage->bytesPerPixel * theImage->numCols;
	}
	else
	{
		printf("Reading of image file failed.\n");
		exit(-1);
	}
	return theImage;
}
