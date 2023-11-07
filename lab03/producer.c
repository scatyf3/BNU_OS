#include <stdio.h>
#include <sys/semaphore.h>
#include <sys/ipc.h>
#include <string.h>
#include "shared_memory.h"

sem_t* sem_prod;
sem_t* sem_cons;
sem_t* sem_mutex;


// 定义主函数
int main() {
    //创建缓冲区和共享内存
    struct sharedMemory* shm=(struct sharedMemory*)attach_memory_block(SECRET_KEY,sizeof(&shm));
    init_buffer(shm);
    printf("here is shm with address %d\n",shm);
    //初始化共享内存
    sem_unlink(SEM_MUTEX_FNAME);
    sem_unlink(SEM_CONSUMER_FNAME);
    sem_unlink(SEM_PRODUCER_FNAME);
    create_sems();
    // 循环等待用户的输入
    // Loop until user chooses to exit
    bool running=true;
    while (running) {
        // Display menu
        printf("\n==== 生产者者程序 ====\n");
        printf("1. 生产产品\n");
        printf("2. 退出\n");
        printf("3. 删除信号量和共享内存\n");
        printf("====================\n");
        printf("请输入选项：");
        // Read user's choice
        int choice;
        scanf("%d", &choice);

        // Perform the corresponding action based on the user's choice
        switch (choice) {
            case 1:
                // Call the production function to add a product to the buffer
                sem_wait(sem_prod);  // wait for sig from consumer
                sem_wait(sem_mutex);
                if(shm->in!=INVALID){
                    fflush(stdin);
                    fgets(shm->buffer[shm->in],sizeof(shm->buffer[shm->in]),stdin);
                    printf("Writing %s\n", shm->buffer[shm->in]);
                    update_in_ptr(shm);
                }
                else{
                    printf("buffer is full\n");
                }
                printSharedMemory(shm);
                sem_post(sem_cons);  // signal something is produced
                sem_post(sem_mutex);
                break;
            case 2:
                // Set running flag to false and exit the loop
                sem_close(sem_prod);
                sem_close(sem_cons);
                sem_close(sem_mutex);
                running = false;
                detach_memory_block(&shm);
                break;
            case 3:
                // Call the function to delete the semaphores and shared memory
                // Set running flag to false and exit the loop
                sem_close(sem_prod);
                sem_close(sem_cons);
                sem_close(sem_mutex);
                destroy_memory_block(SECRET_KEY);
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
