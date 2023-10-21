#include <queue>
#define N 10

template<typename T>
class buffer_queue {
private:
    std::queue<T> used;
    std::queue<T> empty;
public:
    int* shmid_local[N];
    buffer_queue(){
        for(int i=0;i<N;i++){
            shmid[i]=0;
        }
        std::queue<T> used=std::queue<T>();
        std::queue<T> empty=std::queue<T>();
    }
    void mark_shm_use(int identifer){
        used.enque(identifer);
        empty.deque(identifer);
    }
    void mark_shm_empty(int identifer){
        used.deque(identifer);
        empty.enque(identifer);
    }
    int get_buf_in(){
        return empty.front();
    }
    int get_buf_out(){
        return used.front();
    }
    
};