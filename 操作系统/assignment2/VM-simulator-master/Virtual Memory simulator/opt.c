#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

#define MAXLINE 256

extern int memsize;
extern char * tracefile;
extern int debug;
extern struct frame *coremap;


int traceInstr_pos;
int total_instr_num;

addr_t * trace_list; // head of list of all the trace instructions from file
addr_t * inframe_list; // head of list of all vaddr in physmem



/* Page to evict is chosen using the optimal (aka MIN) algorithm. 
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */
 
int opt_evict() {
	
	int at_end; // if true, the frame_index reached the end of trace file before being referenced
	
	int count; // number of instructions before the wanted frame_index is referenced in trace file
	
	int trace_left; // the instructions that are remaining 
	
	int frame_index = 0; // index we used to search through all frames in inframe_list
	
	int largest_dis = 0; // records the furthest distance
	
	int victim; // frame which we want to evict
	
	int trace_index = traceInstr_pos; // index of trace

	
	while (frame_index < memsize) {
		
		at_end = 1;
		trace_left = total_instr_num - traceInstr_pos;
		trace_index = traceInstr_pos;
		count = 0;
		
		while (trace_left >= 0) { 
			
			/* If the frame list vaddr is the same as the trace vaddr
			 * We know that this frame is referenced 
			 */
			if (inframe_list[frame_index] == trace_list[trace_index]) {
				
				if ( (count > largest_dis) ) {     
					
					largest_dis = count;
					victim = frame_index;

				}
			
			at_end = 0;	// it is referenced before hitting the end, set at_end to false
			break;
			
			}
		
		
		count += 1;	
		trace_left -= 1;
		trace_index += 1;
			
		}
	
		if (at_end) { // if the given frame_index exits loop with at_end being true
					  // we return this as victim frame since this vaddr won't be referenced again
		              
			victim = frame_index;
			inframe_list[victim] = 0; //since we are evicting a frame, we reset the value in our inframe_list
		
			return victim;
		}
	
		else {
			
		frame_index += 1;
	
		}
	
	}
	
	inframe_list[victim] = 0;
				
	return victim;
						
}				 
	


/* This function is called on each access to a page to update any information
 * needed by the opt algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
 
void opt_ref(pgtbl_entry_t *p) {
	
	int frame_num; 
	
	frame_num = (p->frame >> PAGE_SHIFT); //find frame number being referenced
	
	if (!inframe_list[frame_num]) { // if the frame number is "unused" in our list, 
									// record it as used in our list
		
		inframe_list[frame_num] = trace_list[traceInstr_pos];
	
	}
	
	traceInstr_pos += 1; // move the instruction position to next instruction
	
}



/* Initializes any data structures needed for this
 * replacement algorithm.
 */
 
void opt_init() {
	
	FILE * fp; // file stream to read the trace fles in
	
	/*  Used to store the variables when we 
	 *  parse trace file
	 */ 
	char buf[MAXLINE];
	char type;
	addr_t vaddr; 
	
	/* Initalize our globals
	 */
	total_instr_num = 0; // total instruction numbers in trace file
	traceInstr_pos = 0;  // the current position (instruction) in trace 
						 // we are executing
	int index = 0; 
	
	
	inframe_list = malloc(sizeof(addr_t) * memsize); // malloc memory
													 //	for our frame list 	
													// which records all vaddr
																										// that are in physmem	
	while ( index < memsize ) { //populate list
	
		inframe_list[index] = 0;
		index += 1;
	}			

	if((fp = fopen(tracefile, "r")) == NULL) { // open tracefile
		
		perror("Error opening tracefile:");
		exit(1);
		
	}
	
	while(fgets(buf, MAXLINE, fp) != NULL) {  // count total instructions in list

			total_instr_num += 1;
		 
	}
	
	
	trace_list = malloc(total_instr_num * sizeof(addr_t)); //malloc a list to store
															// vaddr of all instructions	
	
	fseek(fp, 0, SEEK_SET); // set file pointer to beginning of file
	
	
	index = 0;
	
	/* Copied from sim.c, parses the trace file for the type and vaddr
	 * In our case, we only want the vaddr to store in our instruction list
	 */
	while( fgets(buf, MAXLINE, fp) != NULL) {  
		
		if(buf[0] != '=') {
			
			sscanf(buf, "%c %lx", &type, &vaddr);
			
			trace_list[index] = vaddr;
			index += 1;
		
		}
	}
	

	fclose(fp); 

}


