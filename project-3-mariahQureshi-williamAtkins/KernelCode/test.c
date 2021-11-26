//Names - William Atkins, Mariah Qureshi
//Professor - Tompkins
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <pthread.h>
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

int DATA_SIZE = 1024;
int NUMBER_INSERTS = 100000;    // NUMBER OF TIMES FOR THE ARRAY
int NUM_INSERTS = 10;		// NUMBER OF INSERTS
char numArray[10][1024];	// BYTES OF DATA, 1024 BYTES EACH


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

  char *data;
  int num_of_inserts = 1;
  long rv;
  
  
  for(int j=0; j<NUMBER_INSERTS; j++) {
    
    data = numArray[j%10];
    
    rv = enqueue_buffer_421_syscall(data);
    
    if (rv < 0) {
      perror("enqueue_buffer_421 syscall failed.");
    } 
    
    else {
      printf("enqueue_buffer_421 syscall ran successfully, check dmesg output. Number of total inserts: %i\n",num_of_inserts);
      num_of_inserts++;
    }
    
  } // End of for loop
  
 pthread_exit(0);
 
} // end of random_insert


//
// Threading Read Function to call DEQUEUE_SYSCALL
void *random_read(void *arg) {

  char readData[DATA_SIZE];
  int num_of_reads = 1;
  long rv;
  
  for(int i = 0; i<NUMBER_INSERTS; i++) {
  
    rv = dequeue_buffer_421_syscall(readData);
   
    if (rv < 0) {
      perror("dequeue_buffer_421_syscall failed.");
    } 
    
    else {
      printf("dequeue_buffer_421 syscall ran successfully, data: %.10s... Number of total reads: %i", readData, num_of_reads);
      num_of_reads++;
      printf("\n");
    }
    
  } // end of for loop
  
  pthread_exit(0);
  
} // end of random_read



//
// Main
int main(int argc, char *argv[]) {

pthread_t writeThread, readThread;	// Used for making write/read threads
long rv; 				// Used for syscall error
char charValue;				// Used for making data

// For Loop to memset 1024 byte data, from 0-9 into global array
for(int j=0; j<10; j++){
	charValue = j + '0';
	memset(numArray[j], charValue, sizeof(numArray[0]));
}
	

//
// Init buffer test
rv = init_buffer_421_syscall();
if(rv < 0) {
	perror("init_buffer_421 syscall failed");
}

else {
  printf("init_buffer_421 ran successfully, check dmesg output\n");
}


// Create threads, then join when completed
pthread_create(&writeThread, NULL, random_insert, NULL);
pthread_create(&readThread, NULL, random_read, NULL);
pthread_join(writeThread, NULL);
pthread_join(readThread, NULL);


// Delete buffer test
rv = delete_buffer_421_syscall();
if(rv < 0) {
	perror("delete_buffer_421 syscall failed");
}

else {
	printf("delete_buffer_421 ran successfully, check dmesg output\n");
}

return 0;

} // End of main

