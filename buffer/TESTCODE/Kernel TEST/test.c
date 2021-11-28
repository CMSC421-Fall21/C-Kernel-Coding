//Names - William Atkins, Mariah Qureshi
//Professor - Tompkins
//Date - 11/27/2021
//Project3 - Threading, Ring Buffer, Systemcalls
//test.c - This is a test file for KERNEL systemcalls

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <time.h>

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

long init_buffer_421_syscall();
long enqueue_buffer_421_syscall(char *data);
long dequeue_buffer_421_syscall(char *data);
long delete_buffer_421_syscall();
void *random_insert(void *arg);
void *random_read(void *arg);

int NUM_INSERTS = 10;		// DEFINE data ELEMENTS
int DATA_SIZE = 1024;		// DEFINE data SIZE
char numArray[10][1024];	// BYTES OF DATA, 1024 BYTES EACH
int NUMBER_INSERTS = 100000;    // NUMBER of ARRAY inserts/reads
int NUM_OF_INSERTS = 0;		// Counts number of inserts
int NUM_OF_READS = 0;		// Counts number of inserts

//
// Init buffer function, call to -> syscall
long init_buffer_421_syscall() {
	return syscall(__NR_init_buffer_421);
}

//
// Insert buffer function, call to -> syscall
long enqueue_buffer_421_syscall(char *data) {
	return syscall(__NR_enqueue_buffer_421,data);
}

//
// Print buffer function, call to -> syscall
long dequeue_buffer_421_syscall(char *data) {
	return syscall(__NR_dequeue_buffer_421,data);
}

//
// Delete buffer function, call to-> syscall
long delete_buffer_421_syscall() {
	return syscall(__NR_delete_buffer_421);
}



//
// Threading Insert Function to call ENQUEUE_SYSCALL
void *random_insert(void *arg) {

  char *data;			// Points to array element to insert
  long rv;			// Error checking
  
  // Try to init the buffer
  rv = init_buffer_421_syscall();
  
  // Run a loop for TOTAL_INSERTS
  for(int j=0; j<NUMBER_INSERTS; j++) {
    
    // Mod to get correct data ELEMENT
    data = numArray[j%10];
    
    // ENQUEUE
    rv = enqueue_buffer_421_syscall(data);
    
    // ERROR CHECKING
    if (rv < 0) {
      perror("enqueue_buffer_421 syscall failed.\n");
    } 
    else {
      printf("enqueue_buffer_421 syscall ran successfully, check dmesg output.\n");
      NUM_OF_INSERTS++;
    }
    
  } // End of NUMBER_INSERTS "for" loop
 
  pthread_exit(0);
 
} // end of random_insert


//
// Threading Read Function to call DEQUEUE_SYSCALL
void *random_read(void *arg) {

  char readData[DATA_SIZE];	// Holds data from buffer
  long rv;			// Error counting
  int countdown;		// Used for unintialized buffer
  float multipler;		// Used for unintialized buffer
  float randomNum;		// Used for sleep()
  
  // Run for loop for number of TOTAL_INSERTS
  for(int i = 0; i<NUMBER_INSERTS; i++) {
  
    // DEQUEUE
    rv = dequeue_buffer_421_syscall(readData);
   
    // ERROR CHECKING
    // -1 == buffer does not exist
    if (rv == -1) {
      
      countdown = 10;	// Set countdown limit
      multipler = 2;	// Set multipler
      
      // Random sleep number
      srand((unsigned int)time(NULL));
      randomNum = (float)rand()/(float)(RAND_MAX) / 100.0;
      
      // Preform countdown before aborting
      while(countdown >= 0){
      printf("dequeue_buffer_421 could not find buffer...waiting: %i\n",countdown);
      
      // RETRY dequeue
      rv = dequeue_buffer_421_syscall(readData);
      	if(rv > 0) {
      		NUM_OF_READS++;
      		break;
      	}
      		
      // Up randomnumber with multipler, then sleep
      randomNum *= multipler;
      sleep(randomNum);		
      	
      // Lower countdown		
      countdown--;
      
      } // End of while loop
      
      
      if (countdown < 0){
      	printf("dequeue_buffer_421...BUFFER COULD NOT BE FOUND! Aborting random_read thread.\n");
      	pthread_exit(0);
      }
      
      
    } // end of -1 error code
    
    // ALL other errors from original dequeue
    else if (rv < 0){
    	perror("dequeue_buffer_421_syscall failed.\n");
    }
    
    // Dequeue success
    else {
      printf("dequeue_buffer_421 syscall ran successfully, data: %.10s...\n", readData);
      NUM_OF_READS++;
    }
    
  } // end of TOTAL_INSERTS for loop
  
 
  // CONSUMER is done, we can delete the buffer
  rv = delete_buffer_421_syscall();
  
  // Delete_buffer Error checking
  if(rv < 0) {
	perror("delete_buffer_421 syscall failed");
  }

   pthread_exit(0);
   
} // end of random_read



//
// Main
int main(int argc, char *argv[]) {

pthread_t writeThread, readThread;	// Used for making write/read threads
char charValue;				// Used for making data

// For Loop to memset 1024 byte data, from 0-9 into global array
for(int j=0; j<10; j++){
	charValue = j + '0';
	memset(numArray[j], charValue, sizeof(numArray[0]));
}
	
// Create threads, then join when completed
pthread_create(&writeThread, NULL, random_insert, NULL);
pthread_create(&readThread, NULL, random_read, NULL);
pthread_join(writeThread, NULL);
pthread_join(readThread, NULL);

printf("Total number of PRODUCER PROCESSES: %i\n",NUM_OF_INSERTS);
printf("Total number of CONSUMER PROCESSES: %i\n",NUM_OF_READS);

return 0;

} // End of main

