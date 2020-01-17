#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#define MAX_LINE 1024 
#define PAGE_NUM 256
#define FRAME_SIZE 256
#define TLB_SIZE 16

int debug = 0;
char buf[MAX_LINE]; //address地址
char *tracefile = NULL;
char *valuefile = NULL; 
int numberframe;
char *replacement_alg = NULL;
/*
// Page table entry (2nd-level). 
typedef struct { 
	unsigned int frame; // if valid bit == 1, physical frame holding vpage
	off_t swap_off;       // offset in swap file of vpage, if any
} pgtbl_entry_t;

struct functions {
	char *name;                  // String name of eviction algorithm
	void (*init)(void);          // Initialize any data needed by alg
	void (*ref)(pgtbl_entry_t *);    // Called on each reference
	int (*evict)();              // Called to choose victim for eviction
};

struct functions algs[] = { 
	{"lru", lru_init, lru_ref, lru_evict},
	{"fifo", fifo_init, fifo_ref, fifo_evict}
};
int num_algs = 2;
void (*init_fcn)() = NULL;
void (*ref_fcn)(pgtbl_entry_t *) = NULL;
int (*evict_fcn)() = NULL;
*/
int find_TLB(int num,int (*tlb)[2])
{
    int i;
    int position = -1;
    for(i=0; i < TLB_SIZE; i++)
    {
        if(tlb[i][0] == num)
        {
            position = i;
            break;
        }
    }
    return position;
}
int main(int argc, char *argv[])
{
	int opt;
	FILE *ftp;
	FILE *value;
	char *usage = "USAGE: sim -f tracefile -b valuefile -n numberframe -p policy-algorithm\n";
	
	while ((opt = getopt(argc, argv, "f:b:n:p:")) != -1) {
		switch (opt) {
		case 'f':
			tracefile = optarg;
			break;
		case 'b':
			valuefile = optarg;
			break;
		case 'n':
			numberframe = (unsigned)strtoul(optarg, NULL, 10);
			break;
		case 'p':
			replacement_alg = optarg;
			break;
		default:
			fprintf(stderr, "%s", usage);
			exit(1);
		}
	}
	
	if(tracefile != NULL) {
		if((ftp = fopen(tracefile, "rb")) == NULL) {
			perror("Error opening tracefile:");
			exit(1);
		}
	}
	if(valuefile != NULL) {
		if((value = fopen(valuefile, "rb")) == NULL) {
			perror("Error opening valuefile:");
			exit(1);
		}
	}
	/*
	// Initialize replacement algorithm functions.
	if(replacement_alg == NULL) {
		fprintf(stderr, "%s", usage);
		exit(1);
	} else {
		int i;
		for (i = 0; i < num_algs; i++) {
			if(strcmp(algs[i].name, replacement_alg) == 0) {
				init_fcn = algs[i].init;
				ref_fcn = algs[i].ref;
				evict_fcn = algs[i].evict;
				break;
			}
		}
		if(evict_fcn == NULL) {
			fprintf(stderr, "Error: invalid replacement algorithm - %s\n", 
					replacement_alg);
			exit(1);
		}
	}
	// Call replacement algorithm's init_fcn before replaying trace.
	init_fcn();
	*/
    char mem[numberframe][FRAME_SIZE]; //内存中数据
	int page_table[numberframe]; //页面表
	int frame_table[numberframe]; //页框表 
//	int empty_frame[numberframe]; //空页框表
	int TLB[TLB_SIZE][2]; //TLB快表

    int i,j;
    memset(page_table,-1,sizeof(page_table)); //各种表结构的初始化
//    for(i=0;i<numberframe;i++) empty_frame[i] = i;
    memset(frame_table,-1,sizeof(frame_table));
    for(j=0;j<2;j++) 
		for(i=0;i<TLB_SIZE;i++) 
			TLB[i][j] = -1;
    
    int virtual_address;
    int physical_address;
    int page_pos;
    int page_offset;
    int frame_pos;
    int frame_offset;
    int Hit_count = 0;
    int TLB_pos = 0;
    int empty = FRAME_SIZE; //剩余空闲页
    int full = 0; //已使用的页
    
    while(fgets(buf,MAX_LINE,ftp) != NULL)
    {
        virtual_address = atoi(buf);
    	if(debug)
    	{
    		printf("%s",buf);
    		printf("%d\n",virtual_address);
		}  
        page_pos = (virtual_address & 0xFF00) >> 8;
        page_offset = virtual_address & 255;
        frame_pos = find_TLB(page_pos,TLB); //是否在快表中
        if (frame_pos == -1)
        {
            // printf("not find in TLB\n");
            TLB[TLB_pos][0] = page_pos;
            if(frame_table[page_pos] == -1)
            {
                if(empty > 0)
                {
                    frame_pos = frame_table[page_pos] = full;
                    page_table[full] = page_pos;
                    fseek(value,page_pos*256,SEEK_SET);
                    fread(mem[full],FRAME_SIZE,1,value); //写入数据
                    full += 1;
                    empty -= 1;
                }
            }
            else
            {
                frame_pos = frame_table[page_pos];
            }
            TLB[TLB_pos][1] = frame_pos;
            TLB_pos = (TLB_pos+1)%16;
        }
        else 
        {
            Hit_count += 1;
            frame_pos = TLB[frame_pos][1];
        }  
		if(debug)     
		{
			printf("%d %d\n",page_pos,page_table[1]);   
			printf("%d %d\n",physical_address,mem[frame_pos][page_offset]);
		}    
        physical_address = frame_pos * 256 + page_offset; 
        printf("%d\n",mem[frame_pos][page_offset]);
    }
    printf("Page Faults = %d\n",full);
    printf("TLB Hits = %d\n",Hit_count);
    return 0;
}
