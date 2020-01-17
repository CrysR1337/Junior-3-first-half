#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define MAX_LINE 1024 
#define PAGE_NUM 256
#define PAGE_SIZE 256
#define FRAME_SIZE 256
#define TLB_SIZE 16

int debug = 0;
int numberframe = 128;
char buf[MAX_LINE];
char *tracefile = NULL;
char *valuefile = NULL; 

typedef struct Double_Link_List
{
    int table[2];
    struct Double_Link_List *next;
    struct Double_Link_List *front;
}DL_List;


DL_List* DL_List_Init(int size);

DL_List* DL_List_find(int num,DL_List *head);

DL_List* DL_List_Update(DL_List *DL_List_head,DL_List *DL_List_pos);

DL_List* DL_List_Delete(DL_List *DL_List_head,DL_List *DL_List_pos);

void Frame_Init(DL_List *head);

int main(int argc, char **argv)
{
    int opt;
    int policy_flag = 1;    
    
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
            numberframe = atoi(optarg);
            if(numberframe>0 && numberframe <= 256);
                // printf("numberframe is %d\n",numberframe);
            else
            {
                printf("Error, numberframe must in(0,256]\n");
                exit(1);
            }
            break;
		case 'p':
			if(strcmp(optarg,"FIFO") == 0 || strcmp(optarg,"fifo") ==0)
            {
                // printf("use FIFO poilcy %s\n",optarg);
                policy_flag = 0;
            }
            else if(strcmp(optarg,"LRU") == 0 || strcmp(optarg,"lru") ==0)
            {
                // printf("use LRU poilcy\n");
            }
            else
            {   
                printf("invaild argument of -p. policy must be LRU or FIFO\n");
                exit(1);
            }
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
        
    char mem[numberframe][FRAME_SIZE];
    int page_table[PAGE_NUM]; //页表，索引为页码，值为帧码
    // int frame_table[numberframe];
    // int empty_frame[numberframe];
    int TLB[TLB_SIZE][2];
    
    int i,j;
    memset(page_table,-1,sizeof(page_table));
    // for(i=0;i<numberframe;i++) empty_frame[i] = i;
    // memset(frame_table,-1,sizeof(frame_table));
    for(j=0;j<2;j++) 
		for(i=0;i<TLB_SIZE;i++) 
			TLB[i][j] = -1;
    
    DL_List *TLB_head = DL_List_Init(TLB_SIZE);
    DL_List *Frame_head = DL_List_Init(numberframe);
    Frame_Init(Frame_head);
    Frame_head = Frame_head->front;
    DL_List *TLB_pos,*Frame_List_pos;
    
    int virtual_address;
    int physical_address;
    int page_pos,page_del;
    int page_offset;
    int frame_pos = 0;
    int frame_offset;
    int Hit_count = 0;
    int empty = FRAME_SIZE;
    int full = 0;
    	
    while(fgets(buf,MAX_LINE,ftp) != NULL)
    {
        virtual_address = atoi(buf);
        page_pos = (virtual_address & 0xFF00) >> 8; //获取页码
        if(debug)
    	{
    		printf("%s",buf);
    		printf("%d\n",virtual_address);
		} 
        page_offset = virtual_address & 255; //获取偏移
        TLB_pos = DL_List_find(page_pos,TLB_head); //查找TLB
        if (TLB_pos == NULL) //TLB未命中
        {
            if (page_table[page_pos] == -1) //页表中无对应帧
            {            
                 Frame_List_pos = DL_List_find(page_pos,Frame_head);
                 if(Frame_List_pos == NULL)
                 {
                    Frame_head = Frame_head->next;
                    page_table[page_pos] = Frame_head->table[1];
                    frame_pos = Frame_head->table[1];
                    page_del = Frame_head->table[0];
                    if(page_del >= 0) 
                    {
                        TLB_pos = DL_List_find(page_del,TLB_head);
                        page_table[page_del] = -1; //从页表中删除对应帧
                    }
                    if(TLB_pos != NULL)
                        TLB_head = DL_List_Delete(TLB_head,TLB_pos); //从TLB中删除对应帧
                    Frame_head ->table[0] = page_pos;
                    // Frame_head ->table[1] = frame_pos;
                    fseek(value,page_pos*256,SEEK_SET); //读取对应帧
                    fread(mem[frame_pos],FRAME_SIZE,1,value);
                    full += 1;
                 }
                 else
                 {
                     frame_pos = Frame_List_pos-> table[1];
                     if(policy_flag)
                         Frame_head =  DL_List_Update(Frame_head,Frame_List_pos); //LRU的页面置换
                 }
                 TLB_head = TLB_head->next;
                 TLB_head ->table[0] = page_pos;
                 TLB_head -> table[1] = frame_pos;
            }
            else
            {
                frame_pos = page_table[page_pos];
                if(policy_flag)
                {
                    Frame_List_pos = DL_List_find(page_pos,Frame_head);
                    Frame_head =  DL_List_Update(Frame_head,Frame_List_pos); //LRU的页面置换
                }

            }
            TLB_head = TLB_head->next;
            TLB_head ->table[0] = page_pos;
            TLB_head -> table[1] = frame_pos;
        }
        else 
        {
            Hit_count += 1;
            frame_pos = TLB_pos->table[1];
            if(policy_flag)
                TLB_head =  DL_List_Update(TLB_head,TLB_pos); //LRU的TLB
        } 
        physical_address = frame_pos * 256 + page_offset;
        printf("%d\n",mem[frame_pos][page_offset]);
    }

    printf("Page Faults = %d\n",full);
    printf("TLB Hits = %d\n",Hit_count);
    return 0;
}

DL_List* DL_List_Init(int size)
{
    int i;
    DL_List *new_node,*cur_node, *head;
    new_node = (DL_List *)malloc(sizeof(DL_List));
    new_node ->table[0] = -1;
    new_node ->table[1] = -1;
    new_node ->front = new_node;
    new_node ->next = new_node;
    head = cur_node = new_node;
    for(i = 0;i < size -1;i++)
    {
        new_node = (DL_List *)malloc(sizeof(DL_List));
        new_node -> table[0] = -1;
        new_node -> table[1] = -1;
        cur_node -> next = new_node;
        new_node->front = cur_node;
        new_node->next = head;
        head ->front = new_node;
        cur_node = new_node;
    }
    return head;
}

DL_List* DL_List_find(int num,DL_List *head)
{
    DL_List *tail = head;
    while(head->next != tail)
    {
        if(head->table[0] == num)
            return head;
        head = head->next;
    }
    if(head->table[0] == num)
        return head;
    return NULL;
}

void Frame_Init(DL_List *head)
{
    DL_List *tail = head;
    int temp = 0;
    while(head->next != tail)
    {
        head->table[1] = temp;
        temp += 1;
        head = head->next;
    }
    head->table[1] = temp;
}

DL_List* DL_List_Update(DL_List *DL_List_head,DL_List *DL_List_pos)
{
    if(DL_List_pos!=DL_List_head)
    {
        DL_List_pos->front->next = DL_List_pos->next; //先从链表中删除要访问的节点
        DL_List_pos->next->front = DL_List_pos->front;
        DL_List_pos->next = DL_List_head->next; //将节点放在头部，头部往下代表最少使用，往上代表最近使用
        DL_List_pos->front = DL_List_head;
        DL_List_head->next->front = DL_List_pos;
        DL_List_head->next = DL_List_pos;
        DL_List_head = DL_List_pos;   
    }
    return DL_List_head;
}

DL_List* DL_List_Delete(DL_List *DL_List_head,DL_List *DL_List_pos)
{
    if(DL_List_pos != DL_List_head)
    {
        DL_List_pos->table[0] = -1;
        DL_List_pos->front->next = DL_List_pos->next;
        DL_List_pos->next->front = DL_List_pos->front;
        DL_List_pos->next = DL_List_head->next;
        DL_List_pos->front = DL_List_head;
        DL_List_head->next->front = DL_List_pos;
        DL_List_head->next = DL_List_pos; 
    }
    else
    {
        DL_List_head->table[0] = -1;
        DL_List_head = DL_List_head->front;
    }
    return DL_List_head;
}
