#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

long init_buffer_421_syscall();
long enqueue_buffer_421_syscall(char *data);
long dequeue_buffer_421_syscall(char *data);
long delete_buffer_421_syscall();


//
// Init buffer function -> syscall
long init_buffer_421_syscall() {
return syscall(__NR_init_buffer_421);
}

//
// Insert buffer function -> syscall
long enqueue_buffer_421_syscall(char *data) {
return syscall(__NR_enqueue_buffer_421,data);
}

//
// Print buffer function -> syscall
long dequeue_buffer_421_syscall(char *data) {
return syscall(__NR_dequeue_buffer_421,data);
}

//
// Delete buffer function -> syscall
long delete_buffer_421_syscall() {
return syscall(__NR_delete_buffer_421);
}



//
// Main
int main(int argc, char *argv[]) {


int NUM_INSERTS = 10;		// NUMBER OF INSERTS
char numArray[10][1024];	// BYTES OF DATA, 1024 BYTES EACH
long rv; 			// Used for syscall error
char charValue;
char *data;

// For Loop to memset 1024 byte data, from 0-9 into global array
	for(int j=0; j<10; j++){
		charValue = j + '0';
		memset(numArray[j], charValue, sizeof(numArray[0]));
	}

printf("This is numarray%s\n",numArray[1]);

//
// Init buffer test
rv = init_buffer_421_syscall();
if(rv < 0) {
	perror("init_buffer_421 syscall failed");
}

else {
	printf("init_buffer_421 ran successfully, check dmesg output\n");
}



// TO DO MAKE FOR LOOP
// Enqueue buffer test
for(int i = 0; i<NUM_INSERTS; i++){
	
	data = numArray[i%10];
	
	//Test code
	//printf("Data sent was %s\n",data);
	
	rv = enqueue_buffer_421_syscall(data);
	if(rv < 0) {
		perror("enqueue_buffer_421 syscall failed");
	}

	else {
		printf("enqueue_buffer_421 ran successfully, check dmesg output\n");
	}


}

// TO DO MAKE WHILE LOOP
// Dequeue buffer test
for(int i = 0; i<NUM_INSERTS; i++){
	
	rv = dequeue_buffer_421_syscall(data);
	if(rv < 0) {
		perror("dequeue_buffer_421 syscall failed");
	}

	else {
		printf("dequeue_buffer_421 ran successfully, check dmesg output\n");
		
		//Test code:
		printf("data returned was %s\n",data);
	}
}


//
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

