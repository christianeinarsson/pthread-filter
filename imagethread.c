#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "imagethread.h"

pthread_mutex_t sync_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  sync_cond = PTHREAD_COND_INITIALIZER;
int sync_max_count = 0;
int sync_count = 0;

pthread_t * createThread(void * function, void * argument) {
	pthread_t * thread;
	thread = (pthread_t *) malloc(sizeof(pthread_t));
	pthread_create(thread, NULL, function, argument);
	return thread;
}

imagethread * createThreadData(int count){
	int i;
	imagethread * threads;
	threads = (imagethread*) malloc(sizeof(imagethread) * count);
	for(i = 0; i < count; i++){
		threads[i].rank = i;
		threads[i].thread_count = count;
	}
	return threads;
}

#define SYNC_MAX_COUNT NUMBER_OF_THREADS+1

void setThreadSyncCount(int count){
	sync_max_count = count+1;
}

void SynchronizationPoint()
{
	/* lock the access to the count */
	pthread_mutex_lock(&sync_lock);

	/* increment the counter */
	sync_count++;

	/* check if we should wait or not */
	if (sync_count < sync_max_count){
		/* wait for the others */
		pthread_cond_wait(&sync_cond, &sync_lock);
	}
	else
	{
		// Reset the synch point to enable reuse
		sync_count = 0;

		/* broadcast that everybody reached the point */
		pthread_cond_broadcast(&sync_cond);
	}

	/* unlocks the mutex - otherwise only one thread
	will be able to return from the routine! */
	pthread_mutex_unlock(&sync_lock);
}
