/*
File: ppmio.c

Implementation of PPM image file IO functions.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ppmio.h"

int read_ppm (const char * fname, int * xpix, int * ypix, int * max, char * data) {
	char ftype[40];
	char ctype[40] = "P6";
	char line[80];
	int errno;

	FILE * fp;
	char * filename;

	int bytesToRead;
	int bytesRead;

	errno = 0;

	filename = strdup(fname);

	if (filename == NULL) filename = "\0";
	fp = fopen (filename, "r");
	if (fp == NULL) {
		fprintf (stderr, "read_ppm failed to open %s: %s\n", filename, strerror (errno));
		getchar();
		exit(1);
	}

	fgets(line, 80, fp);
	sscanf(line, "%s", ftype);

	while (fgets(line, 80, fp) && (line[0] == '#'));

	sscanf(line, "%d%d", xpix, ypix);
	fscanf(fp, "%d\n", max);

	if(*xpix * *ypix > MAX_PIXELS) {
		fprintf (stderr, "Image size is too big\n");
		return 4;
	};

	if (strncmp(ftype, ctype, 2) == 0) {
		bytesToRead = *xpix * *ypix * 3;
		bytesRead = fread (data, sizeof (char), bytesToRead, fp);
		//fprintf(stderr, "bytesRead = %d\n", bytesRead);
		//fprintf(stderr, "bytesToRead = %d\n", bytesToRead);
		if (bytesRead !=  bytesToRead) {
			perror ("Read failed");
			return 2;
		}
	} else {
		fprintf (stderr, "Wrong file format: %s\n", ftype);
	}

	if (fclose (fp) == EOF) {
		perror ("Close failed");
		return 3;
	}
	return 0;

}


int write_ppm (const char * fname, int xpix, int ypix, char * data) {

	FILE * fp;
	char * filename;
	int errnoOut = 0;

	int bytesToWrite;
	int bytesWritten;

	filename = strdup(fname);

	if (filename == NULL){
		//filename = "\0";

		//filename = (char *) malloc(sizeof(char)+1);
		//sprintf(filename, "\0");

		fprintf (stderr, "write_ppm filename is null.\n");
		exit(1);
	}

	fp = fopen(filename, "w");

	if (fp == NULL) {
		fprintf (stderr, "write_ppm failed to open %s: %s\n", filename, strerror (errnoOut));
		getchar();
		exit(1);
	}

	fprintf (fp, "P6\n");
	fprintf (fp, "%d %d 255\n", xpix, ypix);
	bytesToWrite = xpix*ypix*3;
	bytesWritten = fwrite (data, sizeof (char), bytesToWrite, fp);
	//fprintf(stderr, "bytesToWrite = %d\n", bytesToWrite);
	//fprintf(stderr, "bytesWritten = %d\n", bytesWritten);
	if (bytesWritten != bytesToWrite) {
		perror ("Write failed");
		return 2;
	}
	if (fclose (fp) == EOF) {
		perror ("Close failed");
		return 3;
	}
	return 0;
}
