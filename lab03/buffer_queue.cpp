#include <iostream>
#include <vector>
#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

class Buffer {
public:
    Buffer(int N = 10, int buffer_size = 1024) :
        N(N), buffer_size(buffer_size),
        buffer(N, std::vector<char>(buffer_size)),
        in_ptr(0), out_ptr(0) {
            sem_init(&empty, 0, N);
            sem_init(&full, 0, 0);
            shmid = shmget(key, buffer_size * N, IPC_CREAT | 0666);
            if (shmid == -1) {
                perror("shmget");
                exit(-1);
            }
            buffer = (char*)shmat(shmid, NULL, 0);
        }

    void produce(const std::string& data) {
        sem_wait(&empty);
        for (int i = 0; i < buffer_size; ++i) {
            buffer[in_ptr * buffer_size + i] = (i < data.size()) ? data[i] : '\0';
        }
        in_ptr = (in_ptr + 1) % N;
        sem_post(&full);
    }

    std::string consume() {
        sem_wait(&full);
        auto data = std::string(buffer + out_ptr * buffer_size);
        out_ptr = (out_ptr + 1) % N;
        sem_post(&empty);
        return data;
    }

    ~Buffer() {
        shmdt(buffer);
    }

    void remove() {
        shmdt(buffer);
        shmctl(shmid, IPC_RMID, NULL);
    }

private:
    int N;
    int buffer_size;
    char* buffer;
    int in_ptr;
    int out_ptr;
    int shmid;
    key_t key = ftok(".", 'b');
    sem_t empty;
    sem_t full;
};
