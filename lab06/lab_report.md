# 实验内容

通过编写和调试请求页式存储管理的模拟程序以加深对请求页式存储管理方案的理解。

# 实验过程
## 1

首先，我仔细阅读了实验示意图，理解了实验要求。随后我划分了任务，编写了函数的声明

## 2

根据算法，我实现了函数的逻辑

## 3

最后，我增加了`printMemoryState`这个函数，以更直观的展示内存的状态和算法的运行。在主函数中，硬编码了一些虚拟内存地址

# 实验代码

```c
#include <stdio.h>
#include <stdlib.h>

#define N 64 // 页表大小
#define M 4 // 内存块数

struct PageTableEntry {
    int lNumber; // 页号
    int pNumber; // 物理块号
    int dNumber; // 在磁盘上的位置
    int write; // 修改位标志
    int flag; // 存在位标志
};

struct PageTableEntry page[N]; // 页表
int memory[M]; // 内存块
int pageQueue[M]; // 存放页号的队列

void initPageTable() {
    for (int i = 0; i < N; i++) {
        page[i].lNumber = i;
        page[i].pNumber = -1;
        page[i].dNumber = -1;
        page[i].write = 0;
        page[i].flag = 0;
    }
}

void initMemory() {
    for (int i = 0; i < M; i++) {
        memory[i] = -1;
        pageQueue[i] = -1;
    }
}

int findEmptyMemoryBlock() {
    for (int i = 0; i < M; i++) {
        if (memory[i] == -1)
            return i;
    }
    return -1; // 没有空闲内存块
}

int findPageToReplace() {
    return pageQueue[0];
}

void updatePageQueue(int pageIndex) {
    for (int i = 0; i < M - 1; i++) {
        pageQueue[i] = pageQueue[i+1];
    }
    pageQueue[M-1] = pageIndex;
}

void printMemoryState() {
    printf("Memory State: ");
    for (int i = 0; i < M; i++) {
        if (memory[i] != -1) {
            printf("%d ", memory[i]);
        } else {
            printf("X ");
        }
    }
    printf("\n");
}

void handlePageFault(int pageNumber) {
    int pageIndex = pageNumber >> 10; // 页号为 16~10

    if (page[pageIndex].flag == 1) {
        // 页面已在内存中
        updatePageQueue(pageIndex);
    } else {
        // 页面不在内存中，发生缺页中断
        int emptyBlockIndex = findEmptyMemoryBlock();
        if (emptyBlockIndex != -1) {
            // 有空闲内存块
            memory[emptyBlockIndex] = pageIndex;
            page[pageIndex].pNumber = emptyBlockIndex;
            page[pageIndex].flag = 1;
            updatePageQueue(pageIndex);
        } else {
            // 没有空闲内存块，进行页置换
            int replacePageIndex = findPageToReplace();
            page[replacePageIndex].flag = 0;
            memory[page[replacePageIndex].pNumber] = pageIndex;
            page[pageIndex].pNumber = page[replacePageIndex].pNumber;
            page[pageIndex].flag = 1;
            page[pageIndex].write = 0;
            page[replacePageIndex].pNumber = -1;
            updatePageQueue(pageIndex);
        }

        printf("Page %d loaded into memory.\n", pageNumber);
    }

    printMemoryState();
}

int main() {
    initPageTable();
    initMemory();

    int requests[] = {0x2A3F, 0x1B4C, 0x3E5A, 0x07D6, 0x2F1E, 0x4C9B, 0x6A8D, 0x003F, 0x1A2B, 0x3C4D, 0x7E5F, 0x2A1C, 0x1B3D}; // 请求虚地址序列

    int numRequests = sizeof(requests) / sizeof(requests[0]);

    for (int i = 0; i < numRequests; i++) {
        handlePageFault(requests[i]);
    }

    return 0;
}
```


# 实验结果

```sh
➜  src git:(main) ✗ ./a.out   
Page 10815 loaded into memory.
Memory State: 10 X X X 
Page 6988 loaded into memory.
Memory State: 10 6 X X 
Page 15962 loaded into memory.
Memory State: 10 6 15 X 
Page 2006 loaded into memory.
Memory State: 10 6 15 1 
Page 12062 loaded into memory.
Memory State: 11 6 15 1 
Page 19611 loaded into memory.
Memory State: 11 19 15 1 
Page 27277 loaded into memory.
Memory State: 11 19 26 1 
Page 63 loaded into memory.
Memory State: 11 19 26 0 
Page 6699 loaded into memory.
Memory State: 6 19 26 0 
Page 15437 loaded into memory.
Memory State: 6 15 26 0 
Page 32351 loaded into memory.
Memory State: 6 15 31 0 
Page 10780 loaded into memory.
Memory State: 6 15 31 10 
Memory State: 6 15 31 10 
```
