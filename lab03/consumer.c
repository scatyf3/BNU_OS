//
// Created by Juanita Howe on 11/6/23.
//
#include <stdio.h>
#include "shared_memory.h"


int main() {
    // 获取共享内存标识符和信号量标识符
    struct sharedMemory* shm=(struct sharedMemory*)attach_memory_block(SECRET_KEY,sizeof(&shm));
    

    // 将共享内存附加到进程的地址空间
    char *shm_addr = shmat(shm_id, NULL, 0);
    if (shm_addr == (char *)-1) {
        perror("shmat");
        exit(1);
    }

    // 进入消费者程序的主循环
    int choice = 0;
    while (1) {
        // 打印菜单选项
        printf("\n==== 消费者程序 ====\n");
        printf("1. 消费产品\n");
        printf("2. 退出\n");
        printf("3. 删除信号量和共享内存\n");
        printf("====================\n");
        printf("请输入选项：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // TODO: 实现消费产品的逻辑
                // 从共享内存中读取产品数据并进行处理
                break;
            case 2:
                // 退出进程，但保留信号量和共享内存
                printf("退出消费者进程\n");
                shmdt(shm_addr);  // 分离共享内存
                exit(0);
            case 3:
                // 删除信号量和共享内存
                printf("删除信号量和共享内存\n");
                // TODO: 删除信号量和共享内存的逻辑
                // 使用 semctl 和 shmctl 函数进行显式删除
                break;
            default:
                printf("无效的选项，请重新选择\n");
        }
    }

    return 0;
}