/*
File: blurfilter.h

Declaration of pixel structure and blurfilter function.

*/

#ifndef _BLURFILTER_H_
#define _BLURFILTER_H_

#include "pixel.h"
#include "imagethread.h"

#define MAX_RAD 1000

typedef struct _blurfilterdata {
	int xsize;
	int ysize;
	pixel * src;
	pixel * dst;
	int radius;
	double * w;
	imagethread * imageThreads;
	int rank;
} blurfilterdata;

void * blurfilterwrapper(void * arg);
void blurfilter(const int xsize, const int ysize, pixel * src, pixel * dst, const int radius, const double *w, imagethread * imageThreads, int rank);

#endif
