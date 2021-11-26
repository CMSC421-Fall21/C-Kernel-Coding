#include "buffer.h"
#include <linux/random.h>
#include <linux/delay.h>
#include <linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/math64.h>

static ring_buffer_421_t buffer;
static struct semaphore mutex;
static struct semaphore fill_count;
static struct semaphore empty_count;

SYSCALL_DEFINE0(init_buffer_421){
	
	// Create the root node.
	node_421_t *node;
	node_421_t *curr;
	int i;
	
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read) {
		printk("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	node = (node_421_t *) kmalloc(sizeof(node_421_t),GFP_KERNEL);
	// Create the rest of the nodes, linking them all together.
	
	
	curr = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		curr->next = (node_421_t *) kmalloc(sizeof(node_421_t),GFP_KERNEL);
		curr = curr->next;
	}
	// Complete the chain.
	curr->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	// Initialize your semaphores here.
	sema_init(&mutex, 1);
	sema_init(&fill_count, 0);
	sema_init(&empty_count, SIZE_OF_BUFFER);

	return 1;
	
}

SYSCALL_DEFINE1(enqueue_buffer_421,char*,data){
	// NOTE: You have to modify this function to use semaphores.
	
	int value; 		// Gets value of FILL count
	int randomNum; 		// Gets a random waittime
	int err;		// Used for error reporting
	
	if (!buffer.write) {
		printk("enqueue_buffer_421(): The buffer does not exist. Aborting.\n");
		
		return -1;
	}
	
	
	// Try to down EMPTY_COUNT semaphore
	value = down_trylock(&empty_count);
	while( value != 0){

		// Get a random Number 1-10;
		get_random_bytes(&randomNum,sizeof(randomNum));
		
		if(randomNum < 0)
			randomNum = randomNum * -1;
		
		randomNum %= 10;
		
		// Sleep milliseconds 
		msleep(randomNum);
		
		// Try to DOWN empty_count
		value = down_trylock(&empty_count);
	}
	
	// Up FILL_COUNT semaphore
	up(&fill_count);
	
	// Lock Mutex for writing
	down(&mutex);
	
	err = copy_from_user(buffer.write->data, data, DATA_LENGTH);
	
	// CHECK ERROR
	if (err < 0){
		printk("enqueue_buffer_421(): The buffer was not able to copy_from_user. Aborting.\n");
		return -1;
	}
	
	// Get NEW value of NUM_IN_BUFFER
	printk(":: Enqueueing element into buffer. ::\n%.10s...\n%i items in the buffer.\n",buffer.write->data,fill_count.count);
	
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;
	
	// Release Mutex
	up(&mutex);
	
	return 1;
	
}

SYSCALL_DEFINE1(dequeue_buffer_421,char*,data){
	// NOTE: Implement this function.
	
	int value; 		// Gets value of EMPTY count
	int randomNum; 		// Gets a random waittime
	int err;		// Used for error reporting
	
	// Check to see if the buffer actually Exists
	if (!buffer.read) {
		printk("dequeue_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	
	// Try to DOWN fill_count 
	value = down_trylock(&fill_count);
	while( value != 0){
	
		// Get a random Number 1-10;
		get_random_bytes(&randomNum,sizeof(randomNum));
		
		if(randomNum < 0)
			randomNum = randomNum * -1;
		
		randomNum %= 10;
		
		// Sleep milliseconds 
		msleep(randomNum);
		
		// Try to DOWN fill_count
		value = down_trylock(&fill_count);
	}
	
	// up EMPTY semaphore
	up(&empty_count);
	
	// Lock Mutex for WRITING
	down(&mutex);
	
	// Copy Data Over
	err = __copy_to_user(data, buffer.read->data, sizeof(buffer.read->data));
	
	// CHECK ERROR
	if (err < 0){
		printk("dequeue_buffer_421(): The buffer was not able to copy_to_user. Aborting.\n");
		return -1;
	}
	
	// Print DEQUEUE Information
	value = SIZE_OF_BUFFER - empty_count.count;
	printk(":: Dequeueing element from buffer. ::\n%.10s...\n%i items in the buffer.\n",buffer.read->data,value);
	
	// Free memory in buffer
	memset(buffer.read->data, 0, DATA_LENGTH);
	
	// Advance the READ pointer
	buffer.read = buffer.read->next;
	buffer.length--;
	
	// Release Mutex
	up(&mutex);

	return 1;
	
}

SYSCALL_DEFINE0(delete_buffer_421){

 	node_421_t *temp;
	node_421_t *curr;
	
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Get rid of all existing nodes.
	curr = buffer.read->next;
	while (curr != buffer.read) {
		temp = curr->next;
		kfree(curr);
		curr = temp;
	}
	  
    
	// Free the final node.
	kfree(curr);
	curr = NULL;
	
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	
	return 1;
}
