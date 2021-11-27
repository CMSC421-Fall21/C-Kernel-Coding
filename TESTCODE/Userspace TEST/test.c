//Names - William Atkins, Mariah Qureshi
//Professor - Tompkins
//Date - 11/27/2021
//Project3 - Threading, Ring Buffer, Producer/Consumer
//test.c - This is a test file for Userspace

#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int NUM_INSERTS = 100000;		// NUMBER OF INSERTS
char numArray[10][1024];  	// BYTES OF DATA, 1024 BYTES EACH

// Threading Insert
void *random_insert( void *arg){

	int insertAt;		// Which Byte to Insert
	long isError;		// Checks Error on enqueue
	long rv; 		// Holds Error code
	
	// Make a buffer
	rv = init_buffer_421();

	// For "X" amount of inserts
    	for (int i=0; i<NUM_INSERTS; i++){
        	
        	// Enqueue Process
   		isError = enqueue_buffer_421( numArray[ i%10 ] );
   		
   		// Check error of enqueue buffer
   		if( isError < 0 ){
   			
   			printf("There was a problem enqueuing, aborting thread.\n");
   			rv = delete_buffer_421();
			if (rv < 0){
				printf("Unexpected problem in random_insert, delete_buffer...aborting\n");
		
			}
			
   			pthread_exit (0);
   		}
   	
   		
   	} // End of for loop

}

// Threading Read
void *random_read( void *arg){

	char data[1024];	// Stores the value of string from buffer
	long isError;		// Checks the error
	long rv; 		// Holds Error code
	
	data[1024] = '\0'; 	// Needed
	
	// Make a buffer
	rv = init_buffer_421();
	
	// For "X" amount of reads
    	for (int i=0; i<NUM_INSERTS; i++){
		
		// Dequeue process
		isError = dequeue_buffer_421(data);
		
		// Error check
   		if ( isError < 0 ){
   			printf("There was a problem dequeuing, aborting thread.\n");
   			rv = delete_buffer_421();
   			if (rv < 0){
				printf("Unexpected problem in random_read, delete_buffer, for loop...aborting\n");
		
			}
   			
   			pthread_exit (0);
   		}
   			
	}
	
	// Delete Buffer
	rv = delete_buffer_421();
	if (rv < 0){
		printf("Unexpected problem in random_read, delete_buffer, end of function...aborting\n");
		pthread_exit (0);
	}
	
}



int main(){

	pthread_t writeThread, readThread;	// Threads for writing and reading
	char charValue;				// Char Variable to MEMSET 1024 Bytes of data
	
	// For Loop to memset 1024 byte data, from 0-9 into global array
	for(int j=0; j<10; j++){
		charValue = j + '0';
		memset(numArray[j], charValue, sizeof(numArray[j]));
	}

	// Thread for Insert and Read
	pthread_create(&writeThread, NULL, random_insert, NULL);
	pthread_create(&readThread, NULL, random_read, NULL);
	
	// Join threads to NULL after finishing
	pthread_join(writeThread, NULL);
	pthread_join(readThread, NULL);
	
	// PRINT number of Producers and Consumers processes
	print_global();
	
return 0;

}
