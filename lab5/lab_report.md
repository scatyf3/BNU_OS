# 实验内容

编写C程序，模拟实现动态分区存储管理方案。要求实现首次/最佳适应算法的内存块分配和回收，假设初始状态下，可用内存空间是一块连续区域，大小为102400B。要求实现的功能有：

（1）分配内存

（2）回收内存

（3）显示内存（显示空闲分区和已分配分区的情况）

# 实验过程

## 1

首先，我根据实验要求，搭建出框架和主函数。

## 2

我编写了首次适应算法和最佳适应算法的核心逻辑

## 3

测试bug并且提交

# 实验代码

见src文件夹，这里展示出最核心的两个函数

```c
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
```

# 实验结果

我们可以成功分配内存并显示结果

![Alt text](image.png)

可以回收内存

![Alt text](image-1.png)

# 实验反思

在这个实验中，我们学习了内存管理算法，并实现了首次适应算法和最佳适应算法来分配和回收内存。通过这个实验，我对内存管理算法有了更深入的了解，并且学到了一些重要的经验教训。

通过实现首次适应算法和最佳适应算法，我更清楚地理解了它们的原理和工作方式。首次适应算法在空闲内存块列表中找到第一个符合要求的块进行分配，而最佳适应算法则是在所有可用块中找到最小的合适块来进行分配。这两种算法各有优缺点，了解它们的特点有助于选择适合特定场景的算法。

另外，我好奇stdlib中的malloc是如何实现内存管理的，和我们实现的朴素版本有什么区别。我了解到malloc基于堆分配内存，采取的算法包括首次适应、最佳适应和最坏适应算法，它比我们的实现智能和高效，而且在错误处理上更加健壮。

https://www.xiaolincoding.com/os/3_memory/malloc.html#linux-%E8%BF%9B%E7%A8%8B%E7%9A%84%E5%86%85%E5%AD%98%E5%88%86%E5%B8%83%E9%95%BF%E4%BB%80%E4%B9%88%E6%A0%B7