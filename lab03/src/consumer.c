
#include <stdio.h>
#include "shared_memory.h"

sem_t* sem_prod;
sem_t* sem_cons;
sem_t* sem_mutex;


int main() {
    // 获取共享内存标识符和信号量标识符
    struct sharedMemory* shm=(struct sharedMemory*)attach_memory_block(SECRET_KEY,sizeof(&shm));
    printf("here is shm with address %d\n",shm);
    create_sems();
    bool running=true;
    while (running) {
        // 打印菜单选项
        printf("\n==== 消费者程序 ====\n");
        printf("1. 消费产品\n");
        printf("2. 退出\n");
        printf("3. 删除信号量和共享内存\n");
        printf("====================\n");
        printf("请输入选项：");
        int choice = 0;
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sem_wait(sem_cons);  // wait for sig from consumer
                sem_wait(sem_mutex);
                if(shm->out!=INVALID){
                    printf("Reading %s\n", shm->buffer[shm->out]);
                    update_out_ptr(shm);
                }
                else{
                    printf("there is no element in buffer\n");
                }
                printSharedMemory(shm);
                sem_post(sem_prod);  // signal something is produced
                sem_post(sem_mutex);
                break;
            case 2:
                // 退出进程，但保留信号量和共享内存
                running = false;
                sem_close(sem_prod);
                sem_close(sem_cons);
                sem_close(sem_mutex);
                sem_close(sem_cons);
                detach_memory_block(&shm);
                break;
            case 3:
                sem_close(sem_prod);
                sem_close(sem_cons);
                sem_close(sem_mutex);
                destroy_memory_block(SECRET_KEY);
                running = false;
                break;
            default:
                printf("无效的选项，请重新选择\n");
        }
    }
    printf("消费者进程结束。\n");
    // 返回0

    return 0;
}