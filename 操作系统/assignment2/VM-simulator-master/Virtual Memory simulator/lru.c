#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


lru_node * lru_head;

extern int memsize;
extern int debug;
extern struct frame *coremap;


/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict() {
	
	int ret_frame;
	
	ret_frame = lru_head->frame; //return back the head of our list, which is
								 // least recently used frame
	
	return ret_frame;

}


/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */

void lru_ref(pgtbl_entry_t *p) {

	int frame_num;

   	frame_num = (p->frame >> PAGE_SHIFT); //find frame number, and search for it
										  // in our list, if it is not
										  // we add the frame number to the end of 
										  // our list
	
	if (__search_and_move(frame_num) == 0) {
		
		__add_to_end(frame_num);
		
	}
		
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */

void lru_init() {
	
	lru_head = NULL; // initalize list to NULL
	
}


/* Allocates and adds a new node with frame_num to the beginning of the 
 * end of the lru_node list, which the head is pointed to by lru_head
 */

void __add_to_end (int frame_num) {	

	lru_node * new;
	
	if ( !(new = malloc(sizeof(lru_node) )) ) {
		perror("malloc");
		exit(1);
	}
	
	new->frame = frame_num;
	new->next = NULL; 
	
	if (!lru_head) { // if the lru_list is empty
	
		lru_head = new; //make the new node the head
		
	}
	else {  
		
		lru_node * index;	
		index = __search_tail(); // search for the end of the list
		index->next = new; // append to end of list
		
	}
	
}


/*  Searches the lru_list for the last element in the list, 
 *  returns a pointer to that last element
 *  
 *  returns NULL if the list is empty
 */
 
lru_node * __search_tail() {

	lru_node * index;
	index = lru_head;

	while (index->next) {

		index = index->next;

	}
	
	return index;

}


/* Searches the lru list from lru_head (head of the least recently used)
 * to try to find the node with the frame_num.
 * 
 * If found, returns 1 and moves the node to the back of the list
 * If not found, returns 0
 * 
 */

int __search_and_move(int frame_num) {

	int found = 0;   

	if(lru_head == NULL) { // if the list is an empty list, do nothing and return
                            // not found (0)
		return 0; 
	
	}

   	else if (lru_head->next == NULL) { // if the list is a single element

		if ( lru_head->frame == frame_num ) { // if the single element is the element
		                                       // we are looking for 
			return 1;                         // return found (1)

		} 
	
		else {

			return 0;

		}

	}

	lru_node * tailPtr = __search_tail(); // find the end of the list
	lru_node * index = lru_head;        // we start at beginning of the list
	lru_node * prev_index = NULL; 

   
	while(index != NULL) {  // we know the lru_list is >1 element, iterate and look
                             // through the list for the wanted node
                             
		if ( index->frame == frame_num) { // if we find the node
    		
			found = 1; 
			break;

		}

	prev_index = index;  // iterate through the list
	index = index->next;
  
	}                
   
	if (found == 0) { // if not found

		return 0;

	}
  
	else { // if found (found = 1)

		if (tailPtr == index) { // if the end of the list is the node we wanted
		                        // do nothing since it already is at the end of the
                                // list
			return 1;

		}

		else if (!prev_index) { // if the node is found at the beginning of the list

			lru_head = lru_head->next;   
			index->next = tailPtr->next;
			tailPtr->next = index;
	
			return 1;
  
		}

		else { // found node is somewhere in the middle of the list
		
			prev_index->next = index->next;
			index->next = tailPtr->next;
			tailPtr->next = index;

			return 1;
		}	
	}

}     
    
	
	
	


