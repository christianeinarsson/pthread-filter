#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "imagepart.h"
#include "pixel.h"
#include "imagethread.h"

void divide(imagethread *threadData, int count, pixel * sourceImage, pixel * destinationImage, const int xsize, const int ysize){
	int i;
	int partHeight;
	int partHeightLeft;

	partHeight = ysize/count;
	partHeightLeft = ysize%count;

	fprintf(stderr, "Setting structs\n");
	//array.length finns inte i c, enda sättet att få fram det i efterhand är sizeof(array)/sizeof(element)
	for(i = 0; i < count; i++){
		threadData[i].partition = (imagepart*) malloc(sizeof(imagepart));
	}

	fprintf(stderr, "Setting initial height\n");
	//array.length finns inte i c, enda sättet att få fram det i efterhand är sizeof(array)/sizeof(element)
	for(i = 0; i < count; i++){
		threadData[i].partition->width = xsize; //lokal bredd (samma för alla)
		threadData[i].partition->height = partHeight; //lokal höjd (kan variera)
	}

	fprintf(stderr, "Setting corrected height\n");
	for(i = 0; i < partHeightLeft; i++){
		threadData[i].partition->height++; // Justera upp höjden för de sista pixlarna
	}

	fprintf(stderr, "Setting pixel count & bytes\n");
	for(i = 0; i < count; i++){
		// Number of pixels
		threadData[i].partition->pixelCount = threadData[i].partition->width * threadData[i].partition->height;
		// Total number of bytes in the imagepart
		threadData[i].partition->bytes =  threadData[i].partition->pixelCount * sizeof(pixel);
	}

	fprintf(stderr, "Setting ystart, sourcePixels and destinationPixels\n");
	// First part starts at first row
	threadData[0].partition->ystart = 0;
	threadData[0].partition->ystop = threadData[0].partition->height;
	// First part starts at first pixel
	threadData[0].partition->sourcePixels = sourceImage;
	threadData[0].partition->destinationPixels = destinationImage;
	for(i = 1; i < count; i++){
		// Next part starts at last parts ystart plus last parts height
		threadData[i].partition->ystart = threadData[i-1].partition->ystop;
		threadData[i].partition->ystop = threadData[i].partition->ystart + threadData[i].partition->height;
		// Next part starts at last part plus number of bytes
		threadData[i].partition->sourcePixels = threadData[i-1].partition->sourcePixels + threadData[i-1].partition->bytes;
		threadData[i].partition->destinationPixels = threadData[i-1].partition->destinationPixels + threadData[i-1].partition->bytes;
	}

	//for(i = 0; i < count; i++){
	//	printf("\n======== %d ========\n", i);
	//	printf("sourcePixels = %p\n", threadData[i].partition->sourcePixels);
	//	printf("destinationPixels = %p\n", threadData[i].partition->destinationPixels);
	//	printf("height = %d\n", threadData[i].partition->height);
	//	printf("width = %d\n", threadData[i].partition->width);
	//	printf("ystart = %d\n", threadData[i].partition->ystart);
	//	printf("ystop = %d\n", threadData[i].partition->ystop);
	//	printf("pixelCount = %d\n", threadData[i].partition->pixelCount);
	//	printf("bytes = %d\n", threadData[i].partition->bytes);
	//	//printf(" = %d\n", threadData[i].partition->);
	//}
}
