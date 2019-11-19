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


void crop(RasterImage* theImage, int xCoord, int yCoord, int width, int height, unsigned char* croppedImage);

int main(int argc, char** argv){


	//creates struct theImage using imageCreator within RASTER_IMAGE_H
	char* fPath = argv[1];
	RasterImage* theImage = imageCreator(fPath);

  //assume we have a nice user who is using correct input params
  char* outDir = argv[2];
  int xCoord = atoi(argv[3]);
  int yCoord = atoi(argv[4]);
  int width = atoi(argv[5]);
  int height = atoi(argv[6]);

  unsigned int croppedWidth = atoi(argv[5]);
  unsigned int croppedHeight = atoi(argv[6]);
  unsigned char croppedImage[((croppedWidth*croppedHeight)*theImage->bytesPerPixel)];

  int imgSize = (theImage->numCols * theImage->numRows);

  //crop the image
  crop(theImage, xCoord, yCoord, width, height, croppedImage);

  int flag = writeTGA(outDir,croppedImage, croppedHeight, croppedWidth);

  if (flag == 1) printf("Error when writing to file\n");


return 0;
}

/**
 *
 * @param theImage original RasterImage
 * @param xCoord start x coordinate of crop
 * @param yCoord start y coordinate of crop
 * @param width width of cropped image
 * @param height height of cropped image
 * @param croppedImage final RasterImage to be output
 */
void crop(RasterImage* theImage, int xCoord, int yCoord, int width, int height, unsigned char* croppedImage){

    //start at x,y coords
    int bpr  = theImage->bytesPerRow;//4*numCols
    int bpp =  theImage->bytesPerPixel;//4
    int newbpr = bpp*width;

    //find all pixels in cropped image
    for (int i = yCoord; i < (yCoord + height); i++){
        for(int j = xCoord; j < (xCoord + width); j++){
            for(int k = 0; k < 3; k++){
               // complicated way of accessing theImage raster starting at index [0]
               // and storing the pixel of the cropped image there
             croppedImage[(((i-yCoord)*newbpr)+((j-xCoord)*bpp) + k)] = theImage->raster[(i*bpr)+(j*bpp)+k];
            }
        }
    }
}
