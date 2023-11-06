#include <semaphore.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <deque>
#include <iostream>

using deque<string> = std::deque<std::string>

class Buffer {
   public:
    Buffer() {
        //deque<string> buffer_;
        //in = buffer_.begin();
        //out = buffer_.end();
        char *block = attach_memory_block(SHMFileName_,sizeof(bufferArr));


    }

    void produce(const std::string &data) {
        sem_wait(&empty);
        for (int i = 0; i < buffer_size; ++i) {
            buffer[in_ptr * buffer_size + i] =
                (i < data.size()) ? data[i] : '\0';
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

    ~Buffer() { shmdt(buffer); }

    void remove() {
        shmdt(buffer);
        shmctl(shmid, IPC_RMID, NULL);
    }

   private:
    // Varibles
    // const size of buffer
    static constexpr int maxStringSize_ = 1024;
    static constexpr int maxElements_ = 10;
    static constexpr string SHMFileName_="MaoistCat";
    // buffer deque
    deque<string> bufferQueue_;
    char* bufferArr[maxStringSize_][maxElements_];

    // iter
    auto in;
    auto out;
    // shm
    int shmid;
    key_t key = ftok(".", 'b');
    sem_t consume;
    sem_t full;
    sem_t mutex;

    // Functions

    // add string to deque and boundary check
    void AddString(const std::string &str) {
        if (str.size() <= maxStringSize_ && buffer_.size() < maxElements_) {
            buffer_.push_back(str);
        } else {
            std::cout << "Buffer full or string size exceeds limit."
                      << std::endl;
        }
    }
    // shm warp up
    static int get_shared_block(char *filename, int size) {
        key_t key;
        // Request a key
        // The key is linked to a filename, so that other programs can access
        // it.
        key = ftok(filename, 0);
        // The second argument, 0, is the project ID. If you pass 0 as the
        // project ID, ftok() will generate a key based solely on the given
        // filename.
        if (key == IPC_RESULT_ERROR) {
            return IPC_RESULT_ERROR;
        }
        // get shared block --create it if it doesn't exist
        return shmget(key, size, 0666 | IPC_CREAT);
    }

    char *attach_memory_block(char *filename, int size) {
        int shared_block_id = get_shared_block(filename, size);
        char *result;
        if (shared_block_id == IPC_RESULT_ERROR) {
            return NULL;
        }

        // map the shared block into this process's memory
        // and give me a pointer to it
        result = shmat(shared_block_id, NULL, 0);
        if (result == (char *)IPC_RESULT_ERROR) {
            return NULL;
        }
        return result;
    }

    bool detach_memory_block(char *block) {
        return (shmdt(block) != IPC_RESULT_ERROR);
    }

    bool destroy_memory_block(char *filename) {
        int shared_block_id = get_shared_block(filename, 0);
        if (shared_block_id == IPC_RESULT_ERROR) {
            return NULL;
        }
        return (shmctl(shared_block_id, IPC_RMID, NULL) != IPC_RESULT_ERROR);
    }
};
