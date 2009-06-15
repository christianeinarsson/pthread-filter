#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include "thresmain.h"
#include "ppmio.h"
#include "thresfilter.h"
#include "pixel.h"

int thresmain(int argc, char *argv[]) {
	int thread_count;
	int xsize, ysize, colmax;
	pixel * src;
	pixel * dst;
	imagethread * imageThreads;
	thresfilterdata * thresdata;
	pthread_mutex_t thresholdsum_mutex = PTHREAD_MUTEX_INITIALIZER;
	unsigned int thresholdsum = 0;
	int i;
	char * inputFilepath;
	char * outputFilepath;
	struct timespec stime, etime;

	src = allocate_image(MAX_PIXELS);
	dst = allocate_image(MAX_PIXELS);

	/* Take care of the arguments */

	if (argc != 4) {
		fprintf(stderr, "Usage: %s thread_count infile outfile\n", argv[0]);
		return 1;
	}

	thread_count = atoi(argv[1]);
	if(thread_count < 1){
		fprintf(stderr, "Too few threads\n");
		return 1;
	}

	inputFilepath = strdup(argv[2]);
	outputFilepath = strdup(argv[3]);

	/* read file */
	if(read_ppm (inputFilepath, &xsize, &ysize, &colmax, (char *) src) != 0)
		return 1;

	if (colmax > 255) {
		fprintf(stderr, "Too large maximum color-component value\n");
		return 1;
	}

	printf("Creating threads\n");
	imageThreads = createThreadData(thread_count);

	printf("Dividing image\n");
	divide(imageThreads, thread_count, src, dst, xsize, ysize);
	printf("\n");

	printf("Setting arguments\n");
	thresdata = (thresfilterdata*) malloc(sizeof(thresfilterdata) * thread_count);
	for(i = 0; i < thread_count; i++){
		thresdata[i].xsize = xsize;
		thresdata[i].ysize = ysize;
		thresdata[i].src = src;
		thresdata[i].dst = dst;
		thresdata[i].thresholdsum = &thresholdsum;
		thresdata[i].imageThreads = imageThreads;
		thresdata[i].rank = i;
		thresdata[i].thresholdsum_mutex = &thresholdsum_mutex;
	}

	clock_gettime(CLOCK_REALTIME, &stime);
	printf("Starting threads\n");
	setThreadSyncCount(thread_count);
	for(i = 0; i < thread_count; i++){
		//printf("Starting thread %d\n", i);
		imageThreads[i].argument = &thresdata[i];
		imageThreads[i].thread = createThread(thresfilterwrapper, imageThreads[i].argument);
	}

	printf("Threads working on average value\n");
	SynchronizationPoint();
	printf("Average value is %d\n", thresholdsum/(xsize*ysize));
	printf("Threads working on threshold\n");
	SynchronizationPoint();
	printf("Threads done\n");
	clock_gettime(CLOCK_REALTIME, &etime);

	printf("Filtering took: %g secs\n", (etime.tv_sec  - stime.tv_sec) +
		1e-9*(etime.tv_nsec  - stime.tv_nsec)) ;

	/* write result */
	printf("Writing output file\n");

	if(write_ppm (outputFilepath, xsize, ysize, (char *)src) != 0){
		return 1;
	}

	return 0;
}
