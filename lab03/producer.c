#include <stdio.h>
#include "shared_memory.h"
#include <sys/semaphore.h>
#include <sys/ipc.h>
#include <string.h>

sem_t* sem_prod;
sem_t* sem_cons;
sem_t* sem_mutex;


// 定义主函数
int main() {
    //创建缓冲区和共享内存
    struct sharedMemory* shm=(struct sharedMemory*)attach_memory_block(FILENAME,sizeof(shm));
    //初始化共享内存
    create_sems();
    // 循环等待用户的输入
    // Loop until user chooses to exit
    bool running=true;
    char* local_buffer[BUFFERSIZE];
    while (running) {
        // Display menu
        printf("请选择您要执行的操作：\n");
        printf("1. 生产产品\n");
        printf("2. 退出\n");
        printf("3. 删除信号量和共享内存\n");

        // Read user's choice
        char choice;
        scanf(" %c", &choice);

        // Perform the corresponding action based on the user's choice
        switch (choice) {
            case '1':
                // Call the production function to add a product to the buffer
                sem_wait(sem_prod);  // wait for sig from consumer
                fflush(stdin);
                fgets(shm->buffer[shm->in],sizeof(BUFFERSIZE),stdin);
                printf("Writing %s\n", shm->buffer[shm->in]);
                update_in_ptr(shm);
                sem_post(sem_cons);  // signal something is produced
                break;
            case '2':
                // Set running flag to false and exit the loop
                running = false;
                detach_memory_block(&shm);
                break;
            case '3':
                // Call the function to delete the semaphores and shared memory
                // Set running flag to false and exit the loop
                printf("remove\n");
                running = false;
                break;
            default:
                // Display invalid input message
                printf("无效的输入，请重新输入。\n");
                break;
        }
    }
    printf("生产者进程结束。\n");
    // 返回0
    return 0;
}
