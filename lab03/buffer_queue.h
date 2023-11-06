#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <vector>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

class Buffer {
public:
    Buffer(int N = 10, int buffer_size = 1024);
    void produce(const std::string& data);
    std::string consume();
    ~Buffer();
    void remove();

private:
    int N;
    int buffer_size;
    char* buffer;
    int in_ptr;
    int out_ptr;
    int shmid;
    key_t key;
    sem_t empty;
    sem_t full;
};

#endif
