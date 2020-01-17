
/*
 *  mempool.h
 *  mempool
 *
 *  Created by doo on 11-1-11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#define MEMPOOL_SIZE (1024*1024) 
static char g_mempool[MEMPOOL_SIZE];

int g_algorithm;

struct chunk {
	char signature[4]; /* ”OSEX” */
	struct chunk *next; /*ptr. to next chunk*/
	int state; /* 0 – free, 1 - used */
	int size; /* size of this chunk */
} *g_chunk_head;


enum {FIRST = 1, BEST, WORST};
enum {FREE = 0, USED};

void myinit(int flag);
void * mymalloc(int size);
void myfree(void *ptr);



///////////////////////////
void memoryLeak();	// 使用在 myinit 中添加 atexit 调用内存泄漏检测 (简单地显示,没有定位到文件与行)

void threadLock(volatile char *state); // 添加函数状态 (锁)
void threadUnlock(volatile char *state);




//////////////////////////////
// 测试函数
void showMempoolChunkInfo(struct chunk *chp); //
void showMempoolInfo(); // 显示当前 mempool 分配信息



/*
 *  mempool.c
 *  mempool
 *
 *  Created by doo on 11-1-11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mempool.h"


/////////////////////////////////////////////////
// 私有变量 与 函数

int _chunkSize; // struct chunk 大小


void* (*_mymalloc)(int); //malloc 内存分配方式接口, 对应 First/Best/Worst Fit 三种方法
void (*_myfree)(void *); //free	  (没有实现多种算法...)

// mempool内存分配操作
void * _alloc(struct chunk * scp, int flag);
void _free(void *ptr);

// ----------------------------------------
// 三种内存分配算法
//首次匹配(first fit),按分区的先后次序，从头查找，找到符合要求的第一个分区
//最佳适配(best fit),找到其大小与要求相差最小的空闲分区
//最差适配(worst fit),找到最大的空闲分区
//
// First Fit
void * _mymallocFirstFit(int flag);

// Best Fit
void * _mymallocBestFit(int flag);

///Worst Fit
void * _mymallocWorstFit(int flag);



////////////////////////////////////////////////////
// 实现
// public 函数

void myinit(int flag)
{
	static char poolState = 0; // 防止多次初始化
	if (poolState) return;
	
	poolState = 1;
	atexit(memoryLeak); // 检测出内存泄漏
	_chunkSize = sizeof(struct chunk);
	
	g_algorithm = flag;
	g_chunk_head = (struct chunk *)g_mempool;
	strncpy(g_chunk_head->signature, "OSEX", 4);
	g_chunk_head->next = NULL;
	g_chunk_head->state = FREE;
	g_chunk_head->size = MEMPOOL_SIZE - _chunkSize; // 实际可用内存
	
	
	// 内存分配算法选择
	switch (g_algorithm)
	{
		case FIRST:	//first fit
			_mymalloc = _mymallocFirstFit;
			break;
			
		case BEST:	//best fit
			_mymalloc = _mymallocBestFit;
			break;
			
		case WORST:	//worst fit
			_mymalloc = _mymallocWorstFit;
			break;

		default:
			// 报错....
			break;
	}
	
	// 内存 free
	_myfree = _free;

}


void * mymalloc(int flag)
{
	static volatile char state = 0; 
	
	threadLock(&state);
	
	void *p = _mymalloc(flag);
	
	threadUnlock(&state);
	return p;
}



void myfree(void *ptr)
{	
	static volatile char state = 0; 
	
	threadLock(&state);
	_myfree(ptr);
	threadUnlock(&state);
}



//////////////////////////////////////////////////////////////
// priate 函数

void * _alloc(struct chunk * scp, int flag)
{
	//assert(scp);
	
	if (!scp) return NULL;
	
	int freeMem = scp->size;
	
	scp->state = USED;
	scp->size = flag;
	
	struct chunk *p = (struct chunk*)((char *)(scp + 1) + flag);
	memcpy(p->signature, "OSEX", 4);
	p->next = scp->next;
	p->state = FREE;
	p->size = freeMem - _chunkSize - flag;
	
	scp->next = p;
		
	return scp + 1;
}

void _free(void *ptr)
{
	if (!ptr) return;
	
	struct chunk *mp = (struct chunk *)((char *)ptr - _chunkSize);
	
	if (USED == mp->state && 0 == memcmp(mp->signature, "OSEX", 4))
	{
		mp->state = FREE; // "free" 掉当前区块的内存
		
		struct chunk *scp = g_chunk_head;
		struct chunk *mpl = NULL, *mpr = mp->next;; 
		
		// 合并右边空闲内存
		if (FREE == mpr->state)
		{
			mp->size += mpr->size + _chunkSize;
			mp->next = mpr->next;
		}
		
		// 合并左边空闲内存
		while (scp && scp != mp) //查找左边的内存块
		{
			mpl = scp;
			scp = mpl->next;
		}
		// 合并
		if (mpl && FREE == mpl->state)
		{
			mpl->size += mp->size + _chunkSize;
			mpl->next = mp->next;
		}
	}
}


// First fit
void * _mymallocFirstFit(int flag)
{
	struct chunk * scp = g_chunk_head;

	while (scp && ((USED == scp->state) || scp->size < flag + _chunkSize))
	{
		scp = scp->next;
	}
	
	return _alloc(scp, flag);

}


// Best fit
void * _mymallocBestFit(int flag)
{
	struct chunk * scp = g_chunk_head;
	struct chunk * mp = NULL;
	int bestSize = MEMPOOL_SIZE + 1;
	
	while (scp)
	{
		if ((FREE == scp->state) && scp->size > flag + _chunkSize && bestSize > scp->size)
		{
			bestSize = scp->size;
			mp = scp;
		}
		
		scp = scp->next;
	}
	
	return _alloc(mp, flag);
}


// Worst fit
void * _mymallocWorstFit(int flag)
{
	struct chunk * scp = g_chunk_head;
	struct chunk * mp = NULL;
	int worstSize = -1;
	
	while (scp)
	{
		if ((FREE == scp->state) && scp->size > flag + _chunkSize && worstSize < scp->size)
		{
			mp = scp;
		}
		
		scp = scp->next;
	}
	
	return _alloc(mp, flag);
}




/////////////////////////////////
void memoryLeak()
{
	struct chunk *scp = g_chunk_head;
	
	int index = 0;
	while (scp)
	{
		if (USED == scp->state)
		{
			printf("*** memory leak ***===chunk[%d]===%s%p%s%d\n", index++,
				   "address: ", scp, 
				   "\t size: ", scp->size);
		}
		
		scp = scp->next;
	}
}


void threadLock(volatile char *state)
{
	while (*state)
	{
		for (int i=0; i<1000; ++i);
		//sleep(1);
	}
	
	*state = 1;
}

void threadUnlock(volatile char *state)
{
	*state = 0;
}


////////////////////////////////
// 测试函数
//
void showMempoolChunkInfo(struct chunk *scp)
{
	printf("%s%s%p%s%d%s%d",
		   "\n=======chunk======",
		   "\naddress: ", scp,
		   "\n  state: ", scp->state,
		   "\n  size : ", scp->size);
}

void showMempoolInfo()
{
	struct chunk *scp = g_chunk_head;
	
	printf("\n******************** begin ******************\n");
	printf(" %s%d%s%d",
		   "mempool total size: ", MEMPOOL_SIZE,
		   "\n         chunk size: ", _chunkSize);
	int index = 0;
	
	while (scp)
	{
		printf("\n\n[index: %d]", index++);
		showMempoolChunkInfo(scp);
		scp = scp->next;
	}
	
	printf("\n-----------------------end-------------------------\n\n");
}


#include <stdio.h>
#include <stdlib.h>
#include "mempool.h"


int main (int argc, const char * argv[])
{
    
	myinit(BEST);
	
	int *p1 = (int *)mymalloc(sizeof(int) * 100);
	int *p2 = (int *)mymalloc(sizeof(int) * 200);
	int *p3 = (int *)mymalloc(sizeof(int) * 50);

	myfree(p1);
	//myfree(p2); //
	myfree(p3);
	
	
	showMempoolInfo();

    return 0;
}


/* 结果
******************** begin ******************
 mempool total size: 1048576
         chunk size: 24

[index: 0]
=======chunk======
address: 0x1001020c0
  state: 0
  size : 400

[index: 1]        //p2 没有 free 的内存
=======chunk======
address: 0x100102268
  state: 1
  size : 800

[index: 2]
=======chunk======
address: 0x1001025a0
  state: 0
  size : 1047304
-----------------------end-------------------------
// 泄漏 检测  ... 没有定位到文件与行号.
*** memory leak ***===chunk[0]===address: 0x100102268	 size: 800
*/
