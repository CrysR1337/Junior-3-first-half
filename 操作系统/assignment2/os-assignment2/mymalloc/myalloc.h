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


void memoryLeak();	// 使用在 myinit 中添加 atexit 调用内存泄漏检测 (简单地显示,没有定位到文件与行)

void threadLock(volatile char *state); // 添加函数状态 (锁)
void threadUnlock(volatile char *state);


// 测试函数
void showMempoolChunkInfo(struct chunk *chp); //
void showMempoolInfo(); // 显示当前 mempool 分配信息
