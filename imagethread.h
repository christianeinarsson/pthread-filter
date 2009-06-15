#ifndef _IMAGETHREAD_H_
#define _IMAGETHREAD_H_

#include "imagepart.h"
#include <pthread.h>

typedef struct _imagethread {
	int rank;
	int thread_count;
	pthread_t * thread;
	struct _imagepart * partition;
	void * argument;
} imagethread;

int initAttributes();

pthread_t * createThread(void * function, void * argument);
imagethread * createThreadData(int count);
void setThreadSyncCount(int count);
void SynchronizationPoint();

#endif
