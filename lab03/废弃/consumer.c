//消费者消费产品（即从满的缓冲区中取出内容在屏幕上打印出来），然后满的缓冲区变为空的缓冲区；

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "shared_memory.h"
#include<sys/semaphore.h>
#include <sys/ipc.h>

char* buffers[10];
sem_t * sems[10];

#define BLOCK_SIZE 4096

int main(){
    for (int i = 0; i < 10; i++)
    {
        //use i as seed
        buffers[i]=attach_memory_block(i,SHM_SIZE);
        
    }
    sem_t *sem_prod=sem_open(SEM_KEY,1);
    while(true){
        sem_wait(sem_prod);
        if(strlen(block)>0){
            printf("reading : %s\n",block);
            bool done = (strcmp(block,"quit")==0);
            block[0]=0;
            if(done){break;}
        }
        sem_post(sem_cons);
    }
    


}