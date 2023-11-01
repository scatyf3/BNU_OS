#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include "shared_memory.h"
#include <semaphore.h>

#define SHM_SIZE 1024
#define N 10
#define INVAILD_POINTER -1
#define SEM_KEY 114514


char* buffers[10];
sem_t * sem_prod;
bool occupied[10];


void move_pointer_in(int in){
    //valid state 0~9
    int sem_value;
    if(in==9){
        for (int i = 0; i < 10; i++){
            if(occupied[i]==0){
                in=i;
                break;
            }
        }
        in=INVAILD_POINTER;

    }
    else{in++;}
}

void move_pointer_out(int out){
    int sem_value;
    if(out==9){
        for (int i = 0; i < 10; i++){
            if(occupied[i]==1){
                out=i;
                break;
            }
        }
        out=INVAILD_POINTER;

    }
    else{out++;}
}


int main()
{
    /*
    创建一个共享内存来模拟生产者-消费者问题中缓冲队列
    该缓冲队列有N（例如N=10）个缓冲区，每个缓冲区的大小为1024B
    每个生产者和消费者对缓冲区必须互斥访问
    缓冲队列除了N个缓冲区外，还有一个指向第一个空缓冲区的指针in，一个指向第一个满缓冲区的指针out；
    ...
    由第一个生产者创建信号量集和共享内存，其他生产者和消费者可以使用该信号量集和共享内存；
    */
    int option;
    int in=0,out=INVAILD_POINTER;
    for (int i = 0; i < 10; i++)
    {
        //use i as seed
        buffers[i]=attach_memory_block(i,SHM_SIZE);
    }
    sem_prod=sem_open(SEM_KEY,IPC_CREAT, 0660,1);
    /*
    在生产者程序中，可以选择：
    1.  生产产品；
    2.  退出。退出进程，但信号量和共享内存仍然存在，其他生产者进程和消费者进程还可以继续使用；
    3.  删除信号量和共享内存。显性删除信号量和共享内存，后续其他生产者进程和消费者进程都不能使用这些信号量和共享内存。
    */
    while (true)
    {
        printf("请输入你的选择1.  生产产品;2.  退出。3.  删除信号量和共享内存。");
        scanf("%d",&option);
        char local_buffer[SHM_SIZE];
        if (option == 1)
        {
            sem_wait(sems_prod); // wait for sig from consumer
            fgets(buffers[in], SHM_SIZE, stdin);
            sem_post(sems_prod); // signal something is produced
            occupied[in]=1;
            move_pointer_in(in);
            move_pointer_out(out);

        }
        if(option==2){
            for (int i = 0; i < 10; i++){
                detach_memory_block(buffers[i]);
                sem_close(sems_prod);
            }
            break;
        }
        if(option==3){
            for (int i = 0; i < 10; i++){
                destroy_memory_block(buffers[i]);
                sem_unlink(sems_prod);
                
            }
        }
    }
}