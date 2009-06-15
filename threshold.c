//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include "ppm.h"
//#include "pixel.h"
//
//void threshold(const int xsize_in, const int ysize_in, pixel* src)
//{
//	FILE *infile, *outfile;
//	int magic, ok;
//	int radius;
//	int xsize, ysize, colmax;
//	pixel *image;
//	int x,y;
//	int avg, pval, psum, sum = 0;
//
//	/* read file */
//
//	magic = ppm_readmagicnumber(infile);
//	if (magic != 'P'*256+'6') {
//		fprintf(stderr, "Wrong magic number\n");
//		getchar();
//		exit(1);
//	}
//	xsize = ppm_readint(infile);
//	ysize = ppm_readint(infile);
//	colmax = ppm_readint(infile);
//	if (colmax > 255) {
//		fprintf(stderr, "Too large maximum color-component value\n");
//		getchar();
//		exit(1);
//	}
//
//	//image = (pixel *) malloc(sizeof(pixel) * xsize*ysize);
//	image = allocate_image(xsize*ysize);
//	if (!fread(image, sizeof(pixel), xsize*ysize, infile)) {
//		fprintf(stderr, "error in fread\n");
//		getchar();
//		exit(1);
//	}
//
//
//	/* filter */
//
//	for (y=0; y<ysize; y++) {
//		for (x=0; x<xsize; x++) {
//			sum += PIXEL(image,x,y)->r;
//			sum += PIXEL(image,x,y)->g;
//			sum += PIXEL(image,x,y)->b;
//		}
//	}
//	avg = sum/(xsize*ysize);
//	for (y=0; y<ysize; y++) {
//		for (x=0; x<xsize; x++) {
//			psum = PIXEL(image,x,y)->r;
//			psum += PIXEL(image,x,y)->g;
//			psum += PIXEL(image,x,y)->b;
//			if (psum > avg)
//				pval = colmax;
//			else
//				pval = 0;
//			PIXEL(image,x,y)->r = pval;
//			PIXEL(image,x,y)->g = pval;
//			PIXEL(image,x,y)->b = pval;
//		}
//	}
//
//	/* write result */
//
//	fprintf(outfile, "P6 %d %d %d\n", xsize, ysize, colmax);
//	if (!fwrite(image, sizeof(pixel), xsize*ysize, outfile)) {
//		fprintf(stderr, "error in fwrite");
//		getchar();
//		exit(1);
//	}
//
//	exit(0);
//}
