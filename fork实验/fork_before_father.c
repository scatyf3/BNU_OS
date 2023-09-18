#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid1, pid2;
    printf("hello");
    // 创建第一个子进程
    pid1 = fork();
    if (pid1 == 0) {
        // 子进程1的代码
        
        printf("b\n");
    } else {
        // 在父进程中创建第二个子进程
        pid2 = fork();
        if (pid2 == 0) {
            // 子进程2的代码
            printf("c\n");
        } else {
            // 父进程的代码
            printf("a\n");
        }
    }

    return 0;
}