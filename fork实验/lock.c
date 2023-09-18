#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    pid_t pid1, pid2;
    
    // 创建文件描述符用于加锁
    int fd = open("lockfile", O_CREAT | O_RDWR, 0666);
    
    // 加锁
    lockf(fd, F_LOCK, 0);
    
    // 创建第一个子进程
    pid1 = fork();
    
    if (pid1 == 0) {
        // 子进程1的代码
        lockf(fd, F_LOCK, 0); // 加锁
        printf("b\n");
        lockf(fd, F_ULOCK, 0); // 解锁
    } else {
        // 在父进程中创建第二个子进程
        pid2 = fork();
        
        if (pid2 == 0) {
            // 子进程2的代码
            lockf(fd, F_LOCK, 0); // 加锁
            printf("c\n");
            lockf(fd, F_ULOCK, 0); // 解锁
        } else {
            // 父进程的代码
            printf("a\n");
            lockf(fd, F_ULOCK, 0); // 解锁
        }
    }
    
    // 关闭文件描述符
    close(fd);
    
    return 0;
}