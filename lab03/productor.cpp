
#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>
#include<buffer_queue.hpp>


#define SHM_SIZE 1024
#define N 10


int main(){
    /*
    创建一个共享内存来模拟生产者-消费者问题中缓冲队列
    该缓冲队列有N（例如N=10）个缓冲区，每个缓冲区的大小为1024B
    每个生产者和消费者对缓冲区必须互斥访问
    缓冲队列除了N个缓冲区外，还有一个指向第一个空缓冲区的指针in，一个指向第一个满缓冲区的指针out；
    ...
    由第一个生产者创建信号量集和共享内存，其他生产者和消费者可以使用该信号量集和共享内存；
    */
    int option,shmid;
    buffer_queue<int> buffer_queue;

    for(int i=0;i<N;i++){
        shmid=shmget(IPC_PRIVATE, SHM_SIZE, IPC_CREAT | 0666),i;
        shmat(shmid,buffer_queue.shmid_local[i],0);
    }

    /*
    在生产者程序中，可以选择：
    1.  生产产品；
    2.  退出。退出进程，但信号量和共享内存仍然存在，其他生产者进程和消费者进程还可以继续使用；
    3.  删除信号量和共享内存。显性删除信号量和共享内存，后续其他生产者进程和消费者进程都不能使用这些信号量和共享内存。
    */
    while(true){
        std::cin>>option;
        std::cout<<"请输入你的选择1.  生产产品;2.  退出。3.  删除信号量和共享内存。"<<std::endl;
        char local_buffer[SHM_SIZE];
        if(option==1){
            shmid = buffer_queue.get_buf_in();
            std::cin>>local_buffer;
            


        }


   }

}