#ifndef _PIXEL_H_
#define _PIXEL_H_

/* NOTE: This structure must not be padded! */
typedef struct _pixel {
	unsigned char r,g,b;
} pixel;

pixel * pix(pixel* image, const int xx, const int yy, const int xsize);
pixel * pix_trans(pixel* image, const int xx, const int yy, const int ysize);
pixel * allocate_image(int size);

/* Pixel Declaration */
#define PIXEL(image,x,y) ((image)+((y)*(xsize)+(x)))

#endif
