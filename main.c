#include <stdio.h>
#include <time.h>
#include "thresmain.h"
#include "blurmain.h"

int main(int argc, char *argv[]) {
	/* Take care of the arguments */
	int result = 0;
	if (argc == 4){
		printf("Threshold!\n");
		result = thresmain(argc, argv);
	} else if (argc == 5){
		printf("Blur!\n");
		result = blurmain(argc, argv);
	} else {
		fprintf(stderr, "Threshold usage: %s thread_count infile outfile\n", argv[0]);
		fprintf(stderr, "Blur usage: %s thread_count radius infile outfile\n", argv[0]);
		result = 1;
	}
	//getchar();
	exit(result);
}
