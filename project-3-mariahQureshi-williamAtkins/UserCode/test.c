#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


int NUM_INSERTS = 100000;	// NUMBER OF INSERTS
char numArray[10][1024];  	// BYTES OF DATA, 1024 BYTES EACH

void *random_insert( void *arg){

	int insertAt;		// Which Byte to Insert
	long isError;		// Checks Error on enqueue
	
	// For "X" amount of inserts
    	for (int i=0; i<NUM_INSERTS; i++){
        	
   		isError = enqueue_buffer_421( numArray[ i%10 ] );
   		
   		// Check error of enqueue buffer
   		if( isError < 0 ){
   			printf("There was a problem dequeuing, aborting thread.\n");
   			pthread_exit (0);
   		}
   	
   		
   	} // End of for loop

}

void *random_read( void *arg){

	char data[1024];	// Stores the value of string from buffer
	long isError;		// Checks the error

	
	data[1024] = '\0'; // Needed
	
    	for (int i=0; i<NUM_INSERTS; i++){
		
		isError = dequeue_buffer_421(data);
		
		// Error check
   		if ( isError < 0 ){
   			printf("There was a problem dequeuing, aborting thread.\n");
   			pthread_exit (0);
   		}
   			
	}

}



int main(){

	pthread_t writeThread, readThread;	// Threads for writing and reading
	char charValue;				// Char Variable to MEMSET 1024 Bytes of data
	long rv; 				// Holds Error code
	
	// For Loop to memset 1024 byte data, from 0-9 into global array
	for(int j=0; j<10; j++){
		charValue = j + '0';
		memset(numArray[j], charValue, sizeof(numArray[j]));
	}

	// Create WRITE and READ thread
	

	// Make a buffer
	rv = init_buffer_421();

	// Thread for Insert and Read
	pthread_create(&writeThread, NULL, random_insert, NULL);
	pthread_create(&readThread, NULL, random_read, NULL);
	
	
	pthread_join(writeThread, NULL);
	pthread_join(readThread, NULL);
	
	// TEST CODE TO PRINT GLOBAL Producers and Consumers
	print_global();
	
	// Delete Buffer
	rv = delete_buffer_421();
	

return 0;

}
