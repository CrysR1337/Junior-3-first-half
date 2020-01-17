#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

int clock_count; // global clock count

extern int memsize;
extern int debug;
extern struct frame *coremap;


/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict() {
	
	while (1) { 
	
		if ( !((coremap[clock_count].pte)->frame & PG_REF) ) { // if the pagetable entry does not
		                                                       // have reference bit set, we 
															   // know its wasnt recently used
															   // thus we will evict this frame
															   // number	 
		
			coremap[clock_count].pte->frame = (coremap[clock_count].pte->frame | PG_REF ); // set ref bit to 1  
			break; // break out of loop
			
		}
																								
		coremap[clock_count].pte->frame = (coremap[clock_count].pte->frame & (~PG_REF) ); // set ref bit to 0 for all
																						  // pagetable entry, clock passes 
																						  // through	
		clock_count += 1;
		
		if (clock_count == memsize) { // if the clock count goes beyond bounds, reset it
	                                  // to the beginning
			clock_count = 0;
		
		}

	}
	
	return clock_count;
	
}
	

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */

void clock_ref(pgtbl_entry_t *p) {

	p->frame = (p->frame | PG_REF); // mark the given pagetable address as "referenced"
	
}


/* Initialize any data structures needed for this replacement
 * algorithm. 
 */

void clock_init() {
	
	clock_count = 0; // set clock initially for frame 0
	
}

