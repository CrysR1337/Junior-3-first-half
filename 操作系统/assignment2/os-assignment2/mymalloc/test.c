#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h> 
#include "myalloc.h"

int main (int argc, const char * argv[])
{
    pid_t pid = (int)syscall(__NR_getpid);
    printf("PID: %d\n", pid);
    sleep(10); 

    printf("myinit\n");
    myinit(BEST);

    int *p1 = (int *)mymalloc(sizeof(int) * 100);
    int *p2 = (int *)mymalloc(sizeof(int) * 200);
    int *p3 = (int *)mymalloc(sizeof(int) * 50);

    myfree(p1);
    sleep(10);
    //myfree(p2); //

    printf("myfree\n");
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
