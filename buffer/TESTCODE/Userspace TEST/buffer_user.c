//Names - William Atkins, Mariah Qureshi
//Professor - Tompkins
//Date - 11/27/2021
//Project3 - Threading, Ring Buffer, Producer/Consumer
//buffer_user.c - Functions definitions for the buffer
//Includes: initializing, deleting, enqueuing, and dequeuing

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <pthread.h>
#include "buffer.h"
#include <time.h>

static ring_buffer_421_t buffer;	// The actual buffer
static sem_t mutex;			// Mutex used for locking
static sem_t fill_count;		// Semaphore for tracking FILL_COUNT
static sem_t empty_count;		// Semaphore for tracking EMPTY_COUNT
int numberOfInserts = 0;  		// TOTAL number of inserts
int numberOfReads = 0;			// TOTAL Number of reads

long init_buffer_421(void) {
	
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printf("init_buffer_421(): Buffer already exists. Aborting.\n\n");
		return -1;
	}
	
	else{
		srand((unsigned int)time(NULL));
	}

	// Create the root node.
	node_421_t *node;
	node = (node_421_t *) malloc(sizeof(node_421_t));
	
	// Create the rest of the nodes, linking them all together.
	node_421_t *current;
	int i;
	current = node;
	
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		current->next = (node_421_t *) malloc(sizeof(node_421_t));
		current = current->next;
	}
	
	// Complete the chain.
	current->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	sem_init(&mutex, 1, 1);
	sem_init(&fill_count, 1, 0);
	sem_init(&empty_count, 1, SIZE_OF_BUFFER);

	return 0;
}

long enqueue_buffer_421(char * data) {
	// NOTE: You have to modify this function to use semaphores.
	
	int value; // Gets value of FILL count
	float randomNum; // Gets a random waittime
	time_t t;
	
	if (!buffer.write) {
		printf("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Inital Sleep
	randomNum = (float)rand()/(float)(RAND_MAX) / 100.0;
	sleep(randomNum);
	
	// Sleep until the fill_count is not FULL
	sem_getvalue(&fill_count,&value);
	while( value == SIZE_OF_BUFFER){
		
		randomNum = (float)rand()/(float)(RAND_MAX) / 100.0;
		sleep(randomNum);
		sem_getvalue(&fill_count,&value);
	}
	
	// Lock Mutex for writing
	sem_wait(&mutex);
	
	// Up FILL semaphore
	sem_post(&fill_count);
	
	// Copy Data
	memcpy(buffer.write->data, data, DATA_LENGTH);
	
	// Print Enqueue info
	printf(":: Enqueueing element into buffer. ::\n");
	printf("%.10s...\n",buffer.write->data);
	sem_getvalue(&fill_count,&value);
	printf("%i items in the buffer.\n\n",value);
	
	// lower EMPTY semaphore
	sem_wait(&empty_count);
	
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;
	
	// Update total number of inserts
	numberOfInserts++;
	
	// Release Mutex
	sem_post(&mutex);
	
	return 0;
}

long dequeue_buffer_421(char * data) {
	// NOTE: Implement this function.
	
	int value; // Gets value of EMPTY count
	float randomNum; // Gets a random waittime
	
	if (!buffer.read) {
		printf("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		
		return -1;
	}
	
	// Initial millisecond sleep
	randomNum = (float)rand()/(float)(RAND_MAX) / 100.0;
	sleep(randomNum);

	// Wait until the kernel is not EMPTY	
	sem_getvalue(&empty_count,&value);
	while( value == SIZE_OF_BUFFER){
		
		randomNum = (float)rand()/(float)(RAND_MAX) / 100.0;
		sleep(randomNum);
		sem_getvalue(&empty_count,&value);
	}
	
	// Lock Mutex for READING
	sem_wait(&mutex);
	
	// up EMPTY semaphore
	sem_post(&empty_count);
	
	// Copy data back into user
	memcpy(data, buffer.read->data, DATA_LENGTH);
	
	// Print Dequeuing information
   	printf(":: Dequeueing element from buffer. ::\n");
	printf("%.10s...\n",data);
	sem_getvalue(&empty_count,&value);
	printf("%i items in the buffer.\n\n",20 - value);
	
	// Lower FILL semaphore
	sem_wait(&fill_count);
	
	// "Free" memory in buffer
	memset(buffer.read->data, 0, DATA_LENGTH);
	
	// Advance the pointer.
	buffer.read = buffer.read->next;
	buffer.length--;
	
	// Update total number of reads
	numberOfReads++;
	
	// Release Mutex
	sem_post(&mutex);
	
	return 0;
}

long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printf("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Get rid of all existing nodes.
	node_421_t *temp;
	node_421_t *current = buffer.read->next;
	while (current != buffer.read) {
		temp = current->next;
		free(current);
		current = temp;
	}
	
	// Free the final node.
	free(current);
	current = NULL;
	
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	
	// Destroy mutexes/semaphores
	sem_destroy(&mutex);
	sem_destroy(&fill_count);
	sem_destroy(&empty_count);
	
	return 0;
}

void print_semaphores(void) {

	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	
	int value;
	sem_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	return;
}


// Prints total number of Producer and Consumer processes
void print_global(void){

	printf("This is the number of inserts in PRODUCER: %i\n",numberOfInserts);
	printf("This is the number of reads in CONSUMER: %i\n",numberOfReads);

}
