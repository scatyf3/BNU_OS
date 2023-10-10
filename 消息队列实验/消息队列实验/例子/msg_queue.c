#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MSGKEY 75
#define MSGSIZE 256

int CLIENT();
int SERVER();

int main(){
    /*
    用一个程序作为“引子”，先后fork()两个子进程
    父进程执行服务端SERVER程序，两个子进程分别执行客户端CLIENT1程序和客户端CLIENT2程序，进行通信。
    */
    pid_t pid1, pid2;
    pid1 = fork();
    if (pid1 == -1) {
        perror("fork");
        exit(1);
    } else if (pid1 == 0) {
        CLIENT();
        perror("execlp");
        exit(1);
    }
    pid2 = fork();
    if (pid2 == -1) {
        perror("fork");
        exit(1);
    } else if (pid2 == 0) {
        CLIENT();
        perror("execlp");
        exit(1);
    }
    printf("SERVER is running...\n");
    SERVER();
    return 0;
}

/*
由SERVER端创建一个Key为75的消息队列
等待CLIENT1端进程和CLIENT2端进程发来的消息
当收到CLIENT1端消息编号为1的消息
以及收到CLIENT2端消息编号为1的消息后
作为结束信号，删除该队列，并结束SERVER。
其中，SERVER每接受到一个消息后显示一句
“(SERVER *SERVER端pid*) received message *消息的编号*
 from CLIENT *CLIENT端pid*”
 ,然后发送一个返回消息给对应CLIENT端，显示一句“(SERVER *SERVER端pid*) sent return message *消息的编号* of CLIENT *CLIENT端pid*”。
*/


struct s_msg{
    long id;
    char msg_str[MSGSIZE];
};

int SERVER(){
	int msgqid,pid,c1_pid,c2_pid;
    struct s_msg msg1;
    struct s_msg msg2;
    if ((msgqid=msgget(MSGKEY,0777))==-1)     /*建立消息队列失败*/
	{
		printf("This message queue does not exist. \n");
		return 1;
	}
    int flag1,flag2=1;
    char buffer[100];
    pid=getpid();
    while(flag1||flag2){
        c1_pid=(msgqid,&msg1,256,1,0);
        c2_pid=(msgqid,&msg2,256,1,0);
        //todo:get server and client pid somehow

        if(msg1.id==1){
            snprintf(buffer, sizeof(buffer),"(SERVER %d) received message %d from CLIENT %d\n",pid,msg1.id,c1_pid);
            flag1=0;
            //todo:msgsnd
            strcpy(msg1.msg_str, buffer);
            msgsnd(msgqid,&msg1,sizeof(msg1),0);
        }
        if(msg2.id==1){
            snprintf(buffer, sizeof(buffer),"(SERVER %d) received message %d from CLIENT %d\n",pid,msg2.id,c2_pid);
            flag2=0;
            strcpy(msg2.msg_str, buffer);
            msgsnd(msgqid,&msg2,sizeof(msg2),0);
        }
    }

}

int CLIENT(){
    /*
    CLIENT1端使用key为75的消息队列，先后发送消息编号从10到1的消息，然后退出。
    CLIENT1每发送一条消息后显示一句“(CLIENT1 CLIENT1端pid) sent message 消息的编号”,
    然后等待接受SERVER端返回给自己的消息后，显示一句
    “(CLIENT1 CLIENT1端pid) received return message 消息的编号 from SERVER端pid”，
    再发送下一条消息。
    */
    int msgqid;
    if ((msgqid=msgget(MSGKEY,0777))==-1)     /*建立消息队列失败*/
	{
		printf("This message queue does not exist. \n");
		exit(1);
	}

   struct s_msg msg;
   strcpy(msg.msg_str,"Some random msg");
   for(int i=10;i>0;i--){
        // 对消息内容进行赋值
        sprintf(msg.msg_str, "(CLIENT1 %d) sent message %d\n", getpid(), i);
        msgsnd(msgqid, &msg, 1024, 0); // 发送消息 msg 到 msgid 消息队列

        printf("(CLIENT1 %d) sent message %d\n", getpid(), i);

        msgrcv(msgqid, &msg, 1024, 0, 0); // 接收消息
        printf("(CLIENT1 %d) received return message %d from SERVER%d\n", getpid(), i, msg.id);
   }


}