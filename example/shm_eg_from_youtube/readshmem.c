#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include "shared_memory.h"
#include<sys/semaphore.h>
#include <sys/ipc.h>

#define BLOCK_SIZE 4096

int main(int argc,char *argv[]){
    if (argc!= 1) {
        printf("usage - %s no args", argv[0]);
        return -1;
    }
    //set up semaphores
    sem_unlink(SEM_CONSUMER_FNAME);
    sem_unlink(SEM_PRODUCER_FNAME);

    //last argument is init value
    sem_t *sem_prod = sem_open(SEM_PRODUCER_FNAME, IPC_CREAT ,0660,0);
    if(sem_prod == SEM_FAILED){
        perror("sem_open/produce");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_cons = sem_open(SEM_CONSUMER_FNAME, IPC_CREAT ,0660,1);
    if(sem_cons == SEM_FAILED){
        perror("sem_open/produce");
        exit(EXIT_FAILURE);
    }

    //grab the shared memory block
    char *block = attach_memory_block(FILENAME,BLOCK_SIZE);
    if(block ==NULL){
        printf("ERROR: couldn't get block\n");
        return -1;
    }

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
    sem_close(sem_cons);
    sem_close(sem_prod);
    detach_memory_block(block);
    return 0;
}