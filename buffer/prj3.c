#include "buffer.h"
//#include <linux/list.h>
//#include <linux/spinlock.h>
#include <linux/semaphore.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>

static ring_buffer_421_t buffer;
static struct semaphore *mutex;
static struct semaphore *fill_count;
static struct semaphore *empty_count;



SYSCALL_DEFINE0(init_buffer_421){
	// Create the root node.
	node_421_t *node;
	node_421_t *curr;
	int i;
	
	printk("TEST: Inside init_buffer_421\n");
	
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
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
	// TODO
	sema_init(mutex, 1);
	sema_init(fill_count, 0);
	sema_init(empty_count, SIZE_OF_BUFFER);

	return 1;
	
}

SYSCALL_DEFINE1(enqueue_buffer_421,char*,data){
	printk("TEST: Inside enqueue_buffer_421\n");
	return 1;
}

SYSCALL_DEFINE1(dequeue_buffer_421,char*,data){
	printk("TEST: Inside dequeue_buffer_421\n");
	return 1;
}

SYSCALL_DEFINE0(delete_buffer_421){

 	node_421_t *temp;
	
	printk("TEST: Inside delete_buffer_421\n");
	
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printk("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	
	// Get rid of all existing nodes.
	
	node_421_t *curr = buffer.read->next;
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
	
	kfree(mutex);
	kfree(fill_count);
	kfree(empty_count);
	
	return 1;
}
