#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "buffer.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


// NUMBER OF INSERTS
int NUM_INSERTS = 100000;


// BYTES OF DATA, 1024 BYTES EACH
char numArray[10][1024];

void *random_insert( void *arg){

	float randomNum;	// Stores Random Wait
	int insertAt;		// Which Byte to Insert
	long isError;
	
	// For "X" amount of inserts
    	for (int i=0; i<NUM_INSERTS; i++){
        	
        	// SEED random number
		srand((unsigned int)time(NULL));
		randomNum = (float)rand()/(float)(RAND_MAX) * 1.0;
   		sleep(randomNum);
   		
   		// TEST CODE
   		//printf("Enqueue is sending%s\n", numArray[ i%10 ] );
   		
   		isError = enqueue_buffer_421( numArray[ i%10 ] );
   		
   		// TEST CODE
   		//printf("Random Number: %f\n",randomNum);
   		
   		// Retry to enqueue buffer
   		while( isError < 0 ){
   		
   			// SEED random number
			srand((unsigned int)time(NULL));
			randomNum = (float)rand()/(float)(RAND_MAX) * 1.0;
   			sleep(randomNum);
   			
   			isError = enqueue_buffer_421( numArray[ i%10 ] );
   		}
   		
   		//printf("ENQUEUE SUCCESS! \n");
   		
   	} // End of for loop

	
	

}

void *random_read( void *arg){

	int howManyReads = 0;
	char data[1024];
	long isError;
	float randomNum;
	
	data[1024] = '\0'; // Needed
	
	while(howManyReads < NUM_INSERTS){
		
		// SEED random number
		srand((unsigned int)time(NULL));
		randomNum = (float)rand()/(float)(RAND_MAX) * 1.0;
		sleep(randomNum);
		
		// Try to dequeue
		isError = dequeue_buffer_421(data);
		
		// Retry to enqueue buffer
   		while( isError < 0 ){
   		
   			// SEED random number
			srand((unsigned int)time(NULL));
			randomNum = (float)rand()/(float)(RAND_MAX) * 1.0;
   			sleep(randomNum);
   			
   			isError = dequeue_buffer_421(data);
   			
   		}
		
		//printf("DEQUEUE SUCCESS! \n");
		
		howManyReads++;
	}

}



int main(){

	char charValue;	// Char Variable to MEMSET 1024 Bytes of data
	long rv; 	// Holds Error code
	
	// For Loop to memset 1024 byte data, from 0-9 into global array
	for(int j=0; j<10; j++){
		charValue = j + '0';
		memset(numArray[j], charValue, sizeof(numArray[j]));
	}

	// Create WRITE and READ thread
	pthread_t writeThread, readThread;

	printf("Hello World, before threading\n");
	
	// Make a buffer
	rv = init_buffer_421();

	// Thread for Insert
	pthread_create(&writeThread, NULL, random_insert, NULL);
	
	// Thread for read
	pthread_create(&readThread, NULL, random_read, NULL);
	
	
	pthread_join(writeThread, NULL);
	pthread_join(readThread, NULL);
	
	printf("Threads are done\n");
	
	// TEST CODE TO PRINT GLOBAL Producers and Consumers
	print_global();
	
	// Delete Buffer
	rv = delete_buffer_421();
	

return 0;

}
