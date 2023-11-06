#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include<sys/semaphore.h>
#include <sys/shm.h>
#include <stdlib.h>

//shared memory
#define N 10              
#define BUFFERSIZE 1024  
#define FILENAME "this homework melt my brain"
#define IPC_RESULT_ERROR (-1)

//semaphores
#define SEM_PRODUCER_FNAME "/myproducer"
#define SEM_CONSUMER_FNAME "/myconsumer"
#define SEM_MUTEX_FNAME "mutex"

//bool
#define bool int
#define true 1
#define false 0


//attach a shared memory block
//associated with filename
//create it if it doesn't exist
static int get_shared_block(char *filename,int size) {
    key_t key;
    // Request a key
    // The key is linked to a filename, so that other programs can access it.
    key = ftok(filename,0);
    // The second argument, 0, is the project ID. If you pass 0 as the project ID, ftok() will generate a key based solely on the given filename.
    if (key == IPC_RESULT_ERROR){return IPC_RESULT_ERROR;}
    //get shared block --create it if it doesn't exist
    return shmget(key,size,0666| IPC_CREAT);
}

void* attach_memory_block(char *filename,int size){
    int shared_block_id = get_shared_block(filename,size);
    void *result;
    if (shared_block_id == IPC_RESULT_ERROR){return NULL;}

    // map the shared block into this process's memory 
    // and give me a pointer to it
    result = shmat(shared_block_id,NULL,0);
    if (result == (void *)IPC_RESULT_ERROR){return NULL;}
    return result;
}

bool detach_memory_block(void *block) {return (shmdt(block)!=IPC_RESULT_ERROR);}

bool destroy_memory_block( char *filename){
    int shared_block_id = get_shared_block(filename,0);
    if (shared_block_id == IPC_RESULT_ERROR){return false;}
    return (shmctl(shared_block_id,IPC_RMID,NULL) != IPC_RESULT_ERROR);
}
//all of the programs will share these values


struct sharedMemory {            // 共享内存结构
    char buffer[N][BUFFERSIZE];  // 消息存储区
    int in, out;               // 两个指针
};


void create_sems(){
    sem_unlink(SEM_MUTEX_FNAME);
    sem_unlink(SEM_CONSUMER_FNAME);
    sem_unlink(SEM_PRODUCER_FNAME);
    sem_t *sem_prod = sem_open(SEM_PRODUCER_FNAME, IPC_CREAT ,0777,10);
    if(sem_prod == SEM_FAILED){
        perror("sem_open/produce");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_cons = sem_open(SEM_CONSUMER_FNAME, IPC_CREAT ,0777,0);
    if(sem_cons == SEM_FAILED){
        perror("sem_open/produce");
        exit(EXIT_FAILURE);
    }
    sem_t *sem_mutex = sem_open(SEM_MUTEX_FNAME, IPC_CREAT ,0777,1);
    if(sem_cons == SEM_FAILED){
        perror("sem_open/produce");
        exit(EXIT_FAILURE);
    }
}

void update_in_ptr(struct sharedMemory* shm) {
    shm->in = (shm->in + 1) % N;
}

void update_out_ptr(struct sharedMemory* shm) {
    shm->out = (shm->out + 1) % N;
}




#endif
