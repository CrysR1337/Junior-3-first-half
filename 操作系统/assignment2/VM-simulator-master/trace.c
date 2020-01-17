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
int numberframe = 10000;
char buf[MAX_LINE];
char *tracefile = NULL;
char *valuefile = NULL; 
FILE * ftp;

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
    ftp = fopen("addresses-locality.txt","w");

    DL_List *TLB_head = DL_List_Init(TLB_SIZE);
    DL_List *Frame_head = DL_List_Init(numberframe);
    Frame_Init(Frame_head);
    Frame_head = Frame_head->front;
    DL_List *TLB_pos,*Frame_List_pos;

    fclose(ftp);
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
        fprintf(ftp, "%p\n", new_node);
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
