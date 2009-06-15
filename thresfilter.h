/*
  File: thresfilter.h

  Declaration of pixel structure and thresfilter function.
    
 */
#ifndef _THRESFILTER_H_
#define _THRESFILTER_H_
#include "pixel.h"
#include "imagethread.h"

typedef struct _thresfilterdata {
	int xsize;
	int ysize;
	pixel * src;
	pixel * dst;
	unsigned int * thresholdsum;
	imagethread * imageThreads;
	int rank;
	pthread_mutex_t * thresholdsum_mutex;
} thresfilterdata;

void * thresfilterwrapper(void * arg);
void thresfilter(const int xsize, const int ysize, pixel* src, unsigned int * thresholdsum, pthread_mutex_t * thresholdsum_mutex, imagethread * imageThreads, int rank);

#endif
