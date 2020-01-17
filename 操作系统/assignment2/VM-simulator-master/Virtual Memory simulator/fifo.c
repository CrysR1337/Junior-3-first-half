#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;
extern int debug;
int counter;
extern struct frame *coremap;


/* Page to evict is chosen using the fifo algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int fifo_evict() {
	
	int top_stack = counter; 
	
	if (counter == memsize - 1) { //if the counter passes the maximum frame number
		                          // reset to 0
		counter = 0;
		
	}
	
	counter += 1; 
	
	return top_stack;
		
}

/* This function is called on each access to a page to update any information
 * needed by the fifo algorithm.
 * Input: The page table entry for the page that is being accessed.
 */

void fifo_ref(pgtbl_entry_t *p) {

	return;

}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */

void fifo_init() {
	
	counter = 0;
	
}
