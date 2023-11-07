#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include<sys/semaphore.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>

//shared memory
#define N 10
#define BUFFERSIZE 1024
#define SECRET_KEY 114514
#define IPC_RESULT_ERROR (-1)

//semaphores
#define SEM_PRODUCER_FNAME "/myproducer"
#define SEM_CONSUMER_FNAME "/myconsumer"
#define SEM_MUTEX_FNAME "/mutex"

//bool
#define bool int
#define true 1
#define false 0
#define INVALID -1

struct sharedMemory {
    char buffer[N][BUFFERSIZE];
    int in, out;
};

static int get_shared_block(key_t key, int size);
void* attach_memory_block(key_t key, int size);
bool detach_memory_block(void* block);
bool destroy_memory_block(key_t key);

void create_sems();
void update_in_ptr(struct sharedMemory* shm);
void update_out_ptr(struct sharedMemory* shm);
void printSharedMemory(const struct sharedMemory* shm);
void init_buffer(struct sharedMemory* shm);

#endif
