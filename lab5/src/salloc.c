#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 102400

// 内存块结构
typedef struct MemoryBlock {
    int start_address;         // 起始地址
    int size;                  // 大小
    struct MemoryBlock* next;  // 指向下一个节点的指针
} MemoryBlock;

MemoryBlock* free_memory;       // 空闲内存链表的头指针
MemoryBlock* allocated_memory;  // 已分配内存链表的头指针

// 初始化内存
void initialize() {
    free_memory = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    free_memory->start_address = 0;
    free_memory->size = MEMORY_SIZE;
    free_memory->next = NULL;

    allocated_memory = NULL;
}

// 分配内存（首次适应算法）
void firstFitAllocate(int size) {
    MemoryBlock* current = free_memory;
    MemoryBlock* previous = NULL;

    while (current != NULL) {
        if (current->size >= size) {
            // 找到空闲内存块大小大于等于所需大小的节点
            MemoryBlock* allocated = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            allocated->start_address = current->start_address;
            allocated->size = size;
            allocated->next = allocated_memory;
            allocated_memory = allocated;

            current->start_address += size;
            current->size -= size;

            if (current->size == 0) {
                // 删除空闲内存块节点
                if (previous == NULL) {
                    free_memory = current->next;
                } else {
                    previous->next = current->next;
                }
                free(current);
            }

            printf("分配成功，起始地址：%d\n", allocated->start_address);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("分配失败，没有足够的空闲内存\n");
}

// 分配内存（最佳适应算法）
void bestFitAllocate(int size) {
    MemoryBlock* current = free_memory;
    MemoryBlock* previous = NULL;
    MemoryBlock* bestFitBlock = NULL;
    int smallestSize = MEMORY_SIZE;

    while (current != NULL) {
        if (current->size >= size && current->size < smallestSize) {
            bestFitBlock = current;
            smallestSize = current->size;
        }

        previous = current;
        current = current->next;
    }

    if (bestFitBlock != NULL) {
        MemoryBlock* allocated = (MemoryBlock*)malloc(sizeof(MemoryBlock));
        allocated->start_address = bestFitBlock->start_address;
        allocated->size = size;
        allocated->next = allocated_memory;
        allocated_memory = allocated;

        bestFitBlock->start_address += size;
        bestFitBlock->size -= size;

        if (bestFitBlock->size == 0) {
            if (previous == NULL) {
                free_memory = bestFitBlock->next;
            } else {
                previous->next = bestFitBlock->next;
            }
            free(bestFitBlock);
        }

        printf("分配成功，起始地址：%d\n", allocated->start_address);
    } else {
        printf("分配失败，没有足够的空闲内存\n");
    }
}

// 回收内存
void deallocateMemory(int start_address) {
    MemoryBlock* current = allocated_memory;
    MemoryBlock* previous = NULL;

    while (current != NULL) {
        if (current->start_address == start_address) {
            MemoryBlock* deallocated = current;
            MemoryBlock* new_free_block =
                (MemoryBlock*)malloc(sizeof(MemoryBlock));
            new_free_block->start_address = deallocated->start_address;
            new_free_block->size = deallocated->size;
            new_free_block->next = free_memory;
            free_memory = new_free_block;

            if (previous == NULL) {
                allocated_memory = current->next;
            } else {
                previous->next = current->next;
            }

            free(deallocated);
            printf("回收成功，起始地址：%d\n", start_address);
            return;
        }

        previous = current;
        current = current->next;
    }

    printf("回收失败，找不到要回收的内存块\n");
}

// 显示内存
void displayMemory() {
    printf("\n╔════════════════════════════════════════════════╗\n");
    printf("║                  内存状态                     ║\n");
    printf("╠════════════════════════════════════════════════╣\n");

    MemoryBlock* current_block = allocated_memory;
    int block_number = 1;

    while (current_block != NULL) {
        printf("║ 块 %d:  起始地址：%d   大小：%d   状态：已分配 ║\n", block_number, current_block->start_address, current_block->size);
        current_block = current_block->next;
        block_number++;
    }

    current_block = free_memory;
    while (current_block != NULL) {
        printf("║ 块 %d:  起始地址：%d   大小：%d   状态：空闲   ║\n", block_number, current_block->start_address, current_block->size);
        current_block = current_block->next;
        block_number++;
    }

    printf("╚════════════════════════════════════════════════╝\n");
}

// 释放内存链表
void freeMemoryList(MemoryBlock* memory) {
    MemoryBlock* current = memory;
    MemoryBlock* temp;
    while (current != NULL) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    initialize();

    int choice;
    int size;
    int start_address;

    while (1) {
        printf("\n请选择操作：\n");
        printf("╔════════════════════════════════════════════╗\n");
        printf("║                内存管理选项                ║\n");
        printf("╠════════════════════════════════════════════╣\n");
        printf("║ 1. 分配内存（首次适应算法）                ║\n");
        printf("║ 2. 分配内存（最佳适应算法）                ║\n");
        printf("║ 3. 回收内存                                ║\n");
        printf("║ 4. 显示内存                                ║\n");
        printf("║ 0. 退出                                    ║\n");
        printf("╚════════════════════════════════════════════╝\n");
        printf("选择：");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                // 退出程序
                printf("程序已退出\n");
                freeMemoryList(free_memory);
                freeMemoryList(allocated_memory);
                return 0;

            case 1:
                // 分配内存（首次适应算法）
                printf("请输入要分配的内存大小：");
                scanf("%d", &size);
                firstFitAllocate(size);
                break;

            case 2:
                // 分配内存（最佳适应算法）
                printf("请输入要分配的内存大小：");
                scanf("%d", &size);
                bestFitAllocate(size);
                break;

            case 3:
                // 回收内存
                printf("请输入要回收的内存的起始地址：");
                scanf("%d", &start_address);
                deallocateMemory(start_address);
                break;

            case 4:
                // 显示内存
                displayMemory();
                break;

            default:
                printf("无效的选择，请重新输入\n");
                break;
        }
    }
}