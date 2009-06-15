#include <stdio.h>
#include "thresfilter.h"
#include "pixel.h"

void * thresfilterwrapper(void * arg){
	thresfilterdata * data = (thresfilterdata *) arg;
	thresfilter(data->xsize, data->ysize, data->src, data->thresholdsum, data->thresholdsum_mutex, data->imageThreads, data->rank);
	return NULL;
}

void thresfilter(const int xsize, const int ysize, pixel* src, unsigned int * thresholdsum, pthread_mutex_t * thresholdsum_mutex, imagethread * imageThreads, int rank){
#define uint unsigned int 

	uint sum, i, psum, nump, numptot, start, stop;
	struct timespec stime, etime;

	imagethread * thread;
	struct _imagepart * part;
	int ystart, ystop;

	fprintf(stderr, "Hello from thread %d\n", rank);

	thread = &imageThreads[rank];
	part = thread->partition;
	ystart = part->ystart;
	ystop = part->ystop;

	clock_gettime(CLOCK_REALTIME, &stime);

	numptot = xsize * ysize;
	nump = part->pixelCount;
	start = part->width * part->ystart;
	stop = part->width * part->ystart + nump;
	//fprintf(stderr, "%d: %d\t%d\t%d\n", rank, numptot, nump, part->width * part->ystart);
	for(i = start, sum = 0; i < stop; i++) {
		sum += (uint)src[i].r + (uint)src[i].g + (uint)src[i].b;
	}

	pthread_mutex_lock(thresholdsum_mutex);
	//fprintf(stderr, "%d: %d\t%d\n", rank, *thresholdsum, sum);
	*thresholdsum += sum;
	pthread_mutex_unlock(thresholdsum_mutex);

	//fprintf(stderr, "%d: %d\t%d\n", rank, *thresholdsum, sum);
	SynchronizationPoint();

	sum = (*thresholdsum)/numptot;
	//fprintf(stderr, "%d: %d\t%d\n", rank, *thresholdsum, sum);

	for(i = start; i < stop; i++) {
		psum = (uint)src[i].r + (uint)src[i].g + (uint)src[i].b;
		if(sum > psum) {
			src[i].r = src[i].g = src[i].b = 0;
		}
		else {
			src[i].r = src[i].g = src[i].b = 255;
		}
	}
	clock_gettime(CLOCK_REALTIME, &etime);

	printf("Filtering %d took %g secs\n", rank, (etime.tv_sec  - stime.tv_sec) +
		1e-9*(etime.tv_nsec  - stime.tv_nsec)) ;

	SynchronizationPoint();
}
