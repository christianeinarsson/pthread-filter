#include <stdio.h>
#include <stdlib.h>
#include "pixel.h"
#include "ppmio.h"

pixel * pix(pixel* image, const int xx, const int yy, const int xsize)
{
	register int off = xsize*yy + xx;

	if(off >= MAX_PIXELS) {
		fprintf(stderr, "\n Terribly wrong: %d %d %d\n",xx,yy,xsize);
	}
	return (image + off);
}

pixel * pix_trans(pixel* image, const int xx, const int yy, const int ysize)
{
	register int off = ysize*xx + yy;

	if(off >= MAX_PIXELS) {
		fprintf(stderr, "\n Terribly wrong: %d %d %d\n",xx,yy,ysize);
	}
	return (image + off);
}

pixel * allocate_image(int size)
{
	pixel *image;
	image = (pixel *)malloc(sizeof(pixel)*size);
	if (!image) {
		fprintf(stderr, "malloc failed");
		getchar();
		exit(1);
	}
	return image;
}
