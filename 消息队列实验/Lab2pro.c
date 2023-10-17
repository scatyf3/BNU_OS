#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define SERVER_TYPE 1

struct msg_buffer {
    long msg_type;
    char msg_text[1024];
};

int main() {
    key_t key = 75;
    int msgid;
    struct msg_buffer msg;
   
    // 创建消息队列
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }
   
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    } else if (pid == 0) {
        // 子进程作为CLIENT发送消息
        for (int i = 10; i >= 1; i--) {
            sprintf(msg.msg_text, "%d", i);
            msg.msg_type = SERVER_TYPE;
            msgsnd(msgid, &msg, sizeof(msg), 0);
           
            printf("(Client) sent message %s\n", msg.msg_text);
           
            msgrcv(msgid, &msg, sizeof(msg), getpid(), 0);
           
            printf("(Client) received return message %s\n", msg.msg_text);
        }
        
        exit(0);
    } else {
        // 父进程作为SERVER接收消息
        while (1) {
            msgrcv(msgid, &msg, sizeof(msg), SERVER_TYPE, 0);
           
            printf("(Server) received message %s\n", msg.msg_text);
           
            if (strcmp(msg.msg_text, "1") == 0) {
                strcpy(msg.msg_text, "Return message");
                msg.msg_type = getpid();
                msgsnd(msgid, &msg, sizeof(msg), 0);
               
                printf("(Server) sent return message\n");

                break;  // 结束服务器的循环
            } else {
                strcpy(msg.msg_text, "Return message");
                msg.msg_type = getpid();
                msgsnd(msgid, &msg, sizeof(msg), 0);
               
                printf("(Server) sent return message\n");
            }
        }
        
        // 等待子进程退出
        wait(NULL);
        
        // 删除消息队列
        msgctl(msgid, IPC_RMID, NULL);
        
        exit(0);
    }
}
