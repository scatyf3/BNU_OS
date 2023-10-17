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
    pid1=fork();
    //pid2=fork();
    if(pid1==0){
        CLIENT();
    }
    else{
        printf("server is running");
        SERVER();
    }
    //还需要fork一次，但是现在为了简单的目的，只fork一个client

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


/*
请设计消息结构struct msgform，每个消息至少包含消息类型、消息编号、进程pid;
请设计消息类型，保证：服务端可以接收所有客户端发给服务端的消息；
每个客户端只能接收服务端发给自己的返回消息，而不会接收发给其他客户端的消息；
服务端和客户端都不能接收自己发出去的消息。
*/
struct s_msg{
    long id;
    struct msgform{
        int type;
        int msg_id;
        int sender_pid;
    } msgform;
};

//c->s type=1
//s->c type=0

int SERVER(){
    //让server端真正跑起来...
    printf("server is called");
	int msgqid,pid,c1_pid;
    struct s_msg msg;
    printf("i'm before msqid_ds");
    //这个地方有问题，无法继续运行？
    struct msqid_ds buf={0};
    printf("i'm after msqid_ds");

    if ((msgqid=msgget(MSGKEY,0777|IPC_CREAT))==-1)     /*建立消息队列失败*/
	{
		printf("This message queue does not exist in server. \n");
        perror("Server msg_queue Error:");
		return 1;
	}
    pid=getpid();

    printf("server init the server");
    
    while(1){
        printf("server is looping");
        msgrcv(msgqid,&msg,1024,1,0);
        //todo:get server and client pid somehow
        msgctl(msgqid, IPC_STAT, &buf);
        c1_pid=buf.msg_lspid;
        //最后发送消息的pid，即client pid
        //snprintf(buffer, sizeof(buffer),"(SERVER %d) received message %d from CLIENT %d\n",pid,msg.id,c1_pid);
        //todo: server端打印出消息
        printf("(SERVER %d) received message %d from CLIENT %d\n",pid,msg.id,c1_pid);
        msgsnd(msgqid,&msg,sizeof(msg),0);
        if(msg.id==1){
           break; 
        }
    }
    //todo：server端结束运行时，关闭消息队列
}

int CLIENT(){
    /*
    CLIENT1端使用key为75的消息队列，先后发送消息编号从10到1的消息，然后退出。
    CLIENT1每发送一条消息后显示一句“(CLIENT1 CLIENT1端pid) sent message 消息的编号”,
    然后等待接受SERVER端返回给自己的消息后，显示一句
    “(CLIENT1 CLIENT1端pid) received return message 消息的编号 from SERVER端pid”，
    再发送下一条消息。
    */
    int msgqid,s_pid;
    struct s_msg msg;
    struct msqid_ds buf={0};
    if ((msgqid=msgget(MSGKEY,0777))==-1)     /*建立消息队列失败*/
	{
		printf("This message queue does not exist in client. \n");
        perror("Client msg_queue Error:");
		exit(1);
	}
   for(int i=10;i>0;i--){
        // 对消息内容进行赋值
        //sprintf(msg.msg_str, "(CLIENT1 %d) sent message %d\n", getpid(), i);->没必要这样
        msg.id=i;
        //为id赋值
        msgsnd(msgqid, &msg, 1024, 0); // 发送消息 msg 到 msgid 消息队列
        printf("(CLIENT %d) sent message %d\n", getpid(), i);
        msgrcv(msgqid, &msg, 1024, 0, 0); // 接收消息
        msgctl(msgqid, IPC_STAT, &buf);
        s_pid=buf.msg_lspid;
        printf("(CLIENT %d) received return message %d from SERVER%d\n", getpid(), i, s_pid);
   }
}