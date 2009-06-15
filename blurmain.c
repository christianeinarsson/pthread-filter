#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ppmio.h"
#include "blurfilter.h"
#include "gaussw.h"

#include "imagethread.h"
#include "pixel.h"

int blurmain(int argc, char *argv[]) {
	int thread_count;
	int radius;
	int xsize, ysize, colmax;
	pixel * src;
	pixel * dst;
	imagethread * imageThreads;
	blurfilterdata * blurdata;
	int i;
	char * inputFilepath;
	char * outputFilepath;
	struct timespec stime, etime;

	double * w;

	src = allocate_image(MAX_PIXELS);
	dst = allocate_image(MAX_PIXELS);

	w = (double *) malloc(sizeof(double) * MAX_RAD);

	/* Take care of the arguments */

	if (argc != 5) {
		fprintf(stderr, "Usage: %s thread_count radius infile outfile\n", argv[0]);
		return 1;
	}

	thread_count = atoi(argv[1]);
	if(thread_count < 1){
		fprintf(stderr, "Too few threads\n");
		return 1;
	}

	thread_count = atoi(argv[1]);
	radius = atoi(argv[2]);
	if((radius > MAX_RAD) || (radius < 1)) {
		fprintf(stderr, "Radius (%d) must be greater than zero and less then %d\n", radius, MAX_RAD);
		return 1;
	}

	inputFilepath = strdup(argv[3]);
	outputFilepath = strdup(argv[4]);

	/* read file */
	if(read_ppm (inputFilepath, &xsize, &ysize, &colmax, (char *) src) != 0)
		return 1;

	if (colmax > 255) {
		fprintf(stderr, "Too large maximum color-component value\n");
		return 1;
	}

	printf("Has read the image, generating coefficients\n");

	/* filter */
	get_gauss_weights(radius, w);

	printf("Calling filter\n");

	clock_gettime(CLOCK_REALTIME, &stime);

	printf("Creating threads\n");
	imageThreads = createThreadData(thread_count);

	printf("Dividing image\n");
	divide(imageThreads, thread_count, src, dst, xsize, ysize);
	printf("\n");

	printf("Setting arguments\n");
	blurdata = (blurfilterdata*) malloc(sizeof(blurfilterdata) * thread_count);
	for(i = 0; i < thread_count; i++){
		blurdata[i].xsize = xsize;
		blurdata[i].ysize = ysize;
		blurdata[i].src = src;
		blurdata[i].dst = dst;
		blurdata[i].radius = radius;
		blurdata[i].w = w;
		blurdata[i].imageThreads = imageThreads;
		blurdata[i].rank = i;
	}

	printf("Starting threads\n");
	setThreadSyncCount(thread_count);
	for(i = 0; i < thread_count; i++){
		//printf("Starting thread %d\n", i);
		imageThreads[i].argument = &blurdata[i];
		imageThreads[i].thread = createThread(blurfilterwrapper, imageThreads[i].argument);
	}

	printf("Threads working on x-axis\n");
	SynchronizationPoint();
	printf("Threads working on y-axis\n");
	SynchronizationPoint();
	printf("Threads done\n");

	clock_gettime(CLOCK_REALTIME, &etime);

	printf("Filtering took: %g secs\n", (etime.tv_sec  - stime.tv_sec) +
		1e-9*(etime.tv_nsec  - stime.tv_nsec)) ;

	/* write result */
	printf("Writing output file \"%s\"\n", outputFilepath);

	if(write_ppm (outputFilepath, xsize, ysize, (char *)src) != 0){
		return 1;
	}

	return 0;
}
