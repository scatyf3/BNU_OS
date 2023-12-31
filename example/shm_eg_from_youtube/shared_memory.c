#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/semaphore.h>
#include "shared_memory.h"

//file name is a "seed"


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

char * attach_memory_block(char *filename,int size){
    int shared_block_id = get_shared_block(filename,size);
    char *result;
    if (shared_block_id == IPC_RESULT_ERROR){return NULL;}

    // map the shared block into this process's memory 
    // and give me a pointer to it
    result = shmat(shared_block_id,NULL,0);
    if (result == (char *)IPC_RESULT_ERROR){return NULL;}
    return result;
}

bool detach_memory_block(char *block) {return (shmdt(block)!=IPC_RESULT_ERROR);}

bool destroy_memory_block( char *filename){
    int shared_block_id = get_shared_block(filename,0);
    if (shared_block_id == IPC_RESULT_ERROR){return NULL;}
    return (shmctl(shared_block_id,IPC_RMID,NULL) != IPC_RESULT_ERROR);
}