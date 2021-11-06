#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "buffer.h"



// This buffer should have two RINGED LINK LIST
// One ring is called "READ" the other is called "WRITE"
// ring_buffer->write and ring_buffer->read
// These linked lists nodes POINT TO THE SAME MEMORY ADDRESSES
static ring_buffer_421_t buffer;


// For locking when writing
static sem_t mutex;

// Semaphore for if the buffer is full
// Keeps track of fill count
static sem_t fill_count;

// Semaphore for if the buffer is empty
static sem_t empty_count;



long init_buffer_421(void) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printf("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
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
		
	} // At the end of this for loop, a linked list of nodes are created.
	
	// Complete the chain.
	// By having the last_node POINT to the first_node
	current->next = node;
	
	// Have the global variable buffer, point to ROOT node
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	// TODO
		// !!Unkown if works!!
		// 0 = no semaphore access
		// 1 = can access semaphore
	
	
	sem_init(&mutex,1,1);
	sem_init(&fill_count,1,0);
	sem_init(&empty_count,1,SIZE_OF_BUFFER);

	return 0;
}

long enqueue_buffer_421(char * data) {
	// NOTE: You have to modify this function to use semaphores.
	
	// Wait for MUTEX so we can write to buffer
	sem_wait(&mutex);
	
	if (!buffer.write) {
		printf("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Semaphore to check to see if there are empty write slots.
	if (sem_getvalue(&fill_count) == SIZE_OF_BUFFER){
		printf("dequeue_buffer_421(): Buffer is Full. Aborting.\n");
		return -1;
	}
	
	memcpy(buffer.write->data, data, DATA_LENGTH);
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;


	// Change semaphores for fill_count and empty_count
	sem_post(&fill_count);
	sem_wait(&empty_count);

	// Release mutex
	sem_post(&mutex);

	return 0;
}

long dequeue_buffer_421(char * data) {
	// NOTE: Implement this function.


	
	
	
	/*

	Copies 1024 bytes from the read node into the provided buffer data.
	Correctly update the buffer's length and read pointer.
	Note: We cannot dequeue an uninitialized buffer.
	Note: We cannot dequeue an empty buffer. (length == 0). When this
	happens, we should BLOCK the caller (Hint: use a semaphore).
	Returns 0 on success, -1 on failure.

	*/
	
	// Wait for MUTEX so we can write to buffer
	sem_wait(&mutex);
	
	if (!buffer.read) {
		printf("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Semaphore to check to see if buffer actually has data.
	if (sem_getvalue(&empty_count) == SIZE_OF_BUFFER){
		printf("dequeue_buffer_421(): Buffer is empty. Aborting.\n");
		return -1;
	}
	
	data = memcpy(buffer.read->data, data, DATA_LENGTH);
	buffer->read = buffer->read->next;
	buffer.length--;
	
	// Change semaphores for fill_count and empty_count
	sem_wait(&fill_count);
	sem_post(&empty_count);

	// Release mutex
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
	
	
	// Free all semaphores
	sem_destroy(&mutex);
	sem_destroy(&fill_count);
	sem_destroy(&empty_count);
	
	
	return 0;
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int *value;
	sem_getvalue(&mutex, value);
	printf("sem_t mutex = %p\n", value);
	sem_getvalue(&fill_count, value);
	printf("sem_t fill_count = %p\n", value);
	sem_getvalue(&empty_count, value);
	printf("sem_t empty_count = %p\n", value);
	return;
}
