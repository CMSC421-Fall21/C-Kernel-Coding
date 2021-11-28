**UMBCTube and the Bounded Buffer**
_Group Members - William Atkins, Mariah Qureshi_

This project demonstrates the efficiency of code running in kernel space rather than user space, especially with regards to effectively limiting the overhead of system calls and security routines. 

**NOTE -** Test code can be found in the directory called “TESTCODE”, and subdirectories are divided into Kernel and Userspace tests, for part 1 and part 2.

Below is a discussion of the project implementation by file as follows:

**buffer.h**

	Unchanged from provided file. 

**buffer.c** 

This file is the kernel implementation of the UMBCTube buffer. This file is the equivalent of buffer_user.c but with changes to accommodate for kernel space, (printk, msleep, and so forth). More specifically, copy_from_user(), copy_to_user(), get_random_bytes() were utilized instead memcpy(), srand(), and rand() due to kernel implementation.  Some of the functions that were used in buffer_user.c are user level functions, and cannot be implemented at the kernel level.
This is also true for semaphores. A special library called linux/semaphores.h was included.  Semaphores at the kernel level are structs, which have member variables such as “count” and functions such as “up()”, “down()”, “try_downlock()”. 

**buffer_user.c**

This file is the user implementation of the UMBCTube buffer. Within the init function, there’s a check to ensure that the buffer hasn’t already been created. If not, the buffer starts initializing.  Following the initialization of the root node, the rest of the nodes are created and linked together through iterating a for loop.  We utilize the srand() function with time as our seed. The chain of these linked nodes is completed. Then finally there is the initialization of the semaphores.

Within the enqueue function, we first check to ensure that the buffer hasn’t already been created — if it is not created, this prints out to the console an error, while also returning an error code.  We wait a random amount of time in milliseconds before performing operations.  The value of fill_count is checked, this is a semaphore used to track how full the buffer is.  If the buffer contains more than 20 enqueues, there is a while loop to randomly sleep in milliseconds and recheck the fill_count semaphore value. Following this, the empty_count semaphore is lowered (using wait()), and the fill_count semaphore is up, then we wait for the mutex to become available for writing.  Once the mutex is free, we lock it.  We copy the data from the pointer passed, into the appropriate buffer->write node.  Display enqueue information. Advance the pointer to the next node in buffer->write, and then release the mutex. 

The dequeue function relatively follows a similar pattern to enqueue.  There is a check to see if the buffer exists, dequeue initially waits a random amount of time it operates, and reloops until the empty_count semaphore is no longer at max size.  (If empty_count semaphore == buffer_size, then that means that all the elements in the buffer are empty).  The only difference is enqueue, is using buffer.read instead of buffer.write to track the data, and copying the string into the passed pointer, “data” . We also use memset to free/clear the memory in the buffer->read, before advancing the pointer and releasing the mutex. 

The delete_buffer function deletes all existing nodes before resetting the buffer to null and then destroying the semaphores. The producer and consumer processes are incremented respectively in enqueue and dequeue, before being printed in print_global. The print_semaphores function checks the status of the semaphores. There are no memory leaks and/or errors + warnings. 

**test.c** 

This is the test file for verifying the functionality of our implementation of the UMBCTube buffer. To run the test file, enter the following commands:
	make - (from the make file)
./test
After this has been executed, enter the following commands:
sudo dmesg
This will demonstrate the circular buffer output as intended, following the ‘success’ 
messages printed to the system log.

The test.c tests the buffer with 100000, as the number of inserts — you may change this to other numbers as well to test accordingly. The function random_insert threads enqueue by first making the buffer, and threading the process of the enqueue function. The function random_read follows the same pattern, albeit for the dequeue function threading instead. In main, the threads are created before joining to null and the amount of producer + consumer processes printing after their instantiation. 

**additional file information**

Makefiles are provided with all project files. 
