#ifndef _IMAGEPART_H_
#define _IMAGEPART_H_

#include "pixel.h"
#include "imagethread.h"

#include <pthread.h>

typedef struct _imagethread imagethread;

typedef struct _imagepart {
	struct _pixel * sourcePixels; // Points to the first pixel in the source image
	struct _pixel * destinationPixels; // Points to the first pixel in the destination image
	int height; // Individual for eacn part
	int width; // Same for all parts
	int ystart; // What row the part starts at
	int ystop; // What row the part starts at
	int pixelCount; // Number of pixels
	int bytes; // Total number of bytes in the imagepart
} imagepart;

void divide(imagethread *threadData, int count, pixel * sourceImage, pixel * destinationImage, const int xsize, const int ysize);

#endif
