/*
File: blurfilter.c

Implementation of blurfilter function.

*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "blurfilter.h"
#include "ppmio.h"
#include "pixel.h"
#include "imagethread.h"
#include "imagepart.h"

void * blurfilterwrapper(void * arg){
	blurfilterdata * data = (blurfilterdata*) arg;
	blurfilter(data->xsize, data->ysize, data->src, data->dst, data->radius, data->w, data->imageThreads, data->rank);
	return NULL;
}

void blurfilter(const int xsize, const int ysize, pixel * src, pixel * dst, const int radius, const double *w, imagethread * imageThreads, int rank){
	int x,y,x2,y2, wi;
	double r,g,b,n, wc;

	struct timespec stime, etime;

	imagethread * thread;
	struct _imagepart * part;
	int ystart, ystop;
	int i;

	//fprintf(stderr, "Hello from thread %d\n", rank);

	thread = &imageThreads[rank];
	part = thread->partition;
	ystart = part->ystart;
	ystop = part->ystop;

	clock_gettime(CLOCK_REALTIME, &stime);
	for (y=ystart; y<ystop; y++) {
		for (x=0; x<xsize; x++) {
			r = w[0] * pix(src, x, y, xsize)->r;
			g = w[0] * pix(src, x, y, xsize)->g;
			b = w[0] * pix(src, x, y, xsize)->b;
			n = w[0];
			for (wi=1; wi <= radius; wi++) {
				wc = w[wi];
				x2 = x - wi;
				if(x2 >= 0) {
					r += wc * pix(src, x2, y, xsize)->r;
					g += wc * pix(src, x2, y, xsize)->g;
					b += wc * pix(src, x2, y, xsize)->b;
					n += wc;
				}
				x2 = x + wi;
				if(x2 < xsize) {
					r += wc * pix(src, x2, y, xsize)->r;
					g += wc * pix(src, x2, y, xsize)->g;
					b += wc * pix(src, x2, y, xsize)->b;
					n += wc;
				}
			}
			pix_trans(dst,x,y, ysize)->r = r/n;
			pix_trans(dst,x,y, ysize)->g = g/n;
			pix_trans(dst,x,y, ysize)->b = b/n;
		}
	}

	SynchronizationPoint();

	for (y=0; y<xsize; y++) {
		for (x=ystart; x<ystop; x++) {
			r = w[0] * pix(dst, x, y, ysize)->r;
			g = w[0] * pix(dst, x, y, ysize)->g;
			b = w[0] * pix(dst, x, y, ysize)->b;
			n = w[0];
			for (wi=1; wi <= radius; wi++) {
				wc = w[wi];
				x2 = x - wi;
				if(x2 >= 0) {
					r += wc * pix(dst, x2, y, ysize)->r;
					g += wc * pix(dst, x2, y, ysize)->g;
					b += wc * pix(dst, x2, y, ysize)->b;
					n += wc;
				}
				x2 = x + wi;
				if(x2 < ysize) {
					r += wc * pix(dst, x2, y, ysize)->r;
					g += wc * pix(dst, x2, y, ysize)->g;
					b += wc * pix(dst, x2, y, ysize)->b;
					n += wc;
				}
			}
			pix_trans(src,x,y, xsize)->r = r/n;
			pix_trans(src,x,y, xsize)->g = g/n;
			pix_trans(src,x,y, xsize)->b = b/n;
		}
	}
	clock_gettime(CLOCK_REALTIME, &etime);

	printf("Filtering %d took %g secs\n", rank, (etime.tv_sec  - stime.tv_sec) +
		1e-9*(etime.tv_nsec  - stime.tv_nsec)) ;

	SynchronizationPoint();
}
