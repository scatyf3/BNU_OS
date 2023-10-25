# 实验内容

## 实验任务：

使用linux系统提供的系统调用msgget(),msgrev(),msgctl()编制一个长度为1K的消息发送和接受的程序。

## 实验要求：

* 用一个程序作为“引子”，先后fork()两个子进程，父进程执行服务端SERVER程序，两个子进程分别执行客户端CLIENT1程序和客户端CLIENT2程序，进行通信。
* 由SERVER端创建一个Key为75的消息队列，等待CLIENT1端进程和CLIENT2端进程发来的消息。当收到CLIENT1端消息编号为1的消息，以及收到CLIENT2端消息编号为1的消息后，作为结束信号，删除该队列，并结束SERVER。其中，SERVER每接受到一个消息后显示一句“(SERVER SERVER端pid) received message 消息的编号 from CLIENT CLIENT端pid”,然后发送一个返回消息给对应CLIENT端，显示一句“(SERVER SERVER端pid) sent return message 消息的编号 of CLIENT CLIENT端pid”。
* CLIENT1端使用key为75的消息队列，先后发送消息编号从10到1的消息，然后退出。CLIENT1每发送一条消息后显示一句“(CLIENT1 CLIENT1端pid) sent message 消息的编号”,然后等待接受SERVER端返回给自己的消息后，显示一句“(CLIENT1 CLIENT1端pid) received return message 消息的编号 from SERVER端pid”，再发送下一条消息。
* CLIENT2端使用key为75的消息队列，先后发送消息编号从10到1的消息，然后退出。CLIENT2每发送一条消息后显示一句“(CLIENT2 CLIENT2端pid) sent message 消息的编号”,然后等待接受SERVER端返回给自己的消息后，显示一句“(CLIENT2 CLIENT2端pid) received return message 消息的编号 from SERVER端pid”，再发送下一条消息。

# 程序代码

```c

//xiaorong@bnu.edu.cn

#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#define MSGKEY 75

#define StoC1 1
#define StoC2 2

#define CtoS 3


int CLIENT(int arg);
int SERVER();

pid_t  c1_pid,c2_pid;

/*
请设计消息结构struct msgform，每个消息至少包含消息类型、消息编号、进程pmtpye;
请设计消息类型，保证：服务端可以接收所有客户端发给服务端的消息；
每个客户端只能接收服务端发给自己的返回消息，而不会接收发给其他客户端的消息；
服务端和客户端都不能接收自己发出去的消息。
*/
struct msg_info{
    int msg_id;
    int sender_pid;
};

struct s_msg{
    long mtpye;
    struct msg_info msgInfo;
};

int main(){
    /*
    用一个程序作为“引子”，先后fork()两个子进程
    父进程执行服务端SERVER程序，两个子进程分别执行客户端CLIENT1程序和客户端CLIENT2程序，进行通信。
    */
    c1_pid=fork();
    c2_pid=fork();
    if(c1_pid==0 && c2_pid!=0){
        CLIENT(StoC1);
    }
    else if(c2_pid==0 && c1_pid!=0){
        CLIENT(StoC2);
    }
    else if(c2_pid!=0 && c1_pid!=0){
        //printf("server is running");
        SERVER();
    }

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
 ,然后发送一个返回消息给对应CLIENT端，显示一句“(SERVER *SERVER端pmtpye*) sent return message *消息的编号* of CLIENT *CLIENT端pmtpye*”。
*/


int SERVER(){
    //让server端真正跑起来...

	int msgqid,pid;
    struct s_msg rcv_msg_1;
    struct s_msg snd_msg_1;
    struct s_msg rcv_msg_2;
    struct s_msg snd_msg_2;
    snd_msg_1.mtpye=StoC1;
    snd_msg_2.mtpye=StoC2;

    if ((msgqid=msgget(MSGKEY,0777|IPC_CREAT))==-1)     /*建立消息队列失败*/
	{
		printf("This message queue does not exist in server. \n");
        perror("Server msg_queue Error:");
		return 1;
	}
    pid=getpid();
    printf("server is called,my pid is %d\n",pid);
    int counter=10;
    
    while(1){
        printf("server is looping and counter = %d\n",counter);
        msgrcv(msgqid,&rcv_msg_1,sizeof(rcv_msg_1.msgInfo),CtoS,0);
        msgrcv(msgqid,&rcv_msg_2,sizeof(rcv_msg_2.msgInfo),CtoS,0);
        printf("(SERVER %d) received message %d from CLIENT %d\n",pid,rcv_msg_1.msgInfo.msg_id,c1_pid);
        printf("(SERVER %d) received message %d from CLIENT %d\n",pid,rcv_msg_2.msgInfo.msg_id,c2_pid);
        snd_msg_1.msgInfo.msg_id=counter;
        snd_msg_2.msgInfo.msg_id=counter;
        snd_msg_1.msgInfo.sender_pid=getpid();
        snd_msg_2.msgInfo.sender_pid=getpid();
        msgsnd(msgqid,&snd_msg_1,sizeof(snd_msg_1.msgInfo),0);
        msgsnd(msgqid,&snd_msg_2,sizeof(snd_msg_2.msgInfo),0);
        printf("(SERVER %d) send return message %d to CLIENT %d\n",pid,snd_msg_1.msgInfo.msg_id,c1_pid);
        printf("(SERVER %d) send return message %d to CLIENT %d\n",pid,snd_msg_2.msgInfo.msg_id,c2_pid);
        if(rcv_msg_1.msgInfo.msg_id==1 || rcv_msg_2.msgInfo.msg_id==1){
           break; 
        }
        counter--;
    }
    return 0;
}

int CLIENT(int arg){
    /*
    CLIENT1端使用key为75的消息队列，先后发送消息编号从10到1的消息，然后退出。
    CLIENT1每发送一条消息后显示一句“(CLIENT1 CLIENT1端pmtpye) sent message 消息的编号”,
    然后等待接受SERVER端返回给自己的消息后，显示一句
    “(CLIENT1 CLIENT1端pmtpye) received return message 消息的编号 from SERVER端pmtpye”，
    再发送下一条消息。
    */
    printf("client is called, my pmtpye is %d, my arg is %d\n",getpid(),arg);
    int msgqid,s_pid;
    struct s_msg rcv_msg;
    struct s_msg snd_msg;
    snd_msg.mtpye=CtoS;

    while((msgqid=msgget(MSGKEY,0777))==-1);

    for(int i=10;i>0;i--){
        // 对消息内容进行赋值
        //sprintf(msg.msg_str, "(CLIENT1 %d) sent message %d\n", getpmtpye(), i);->没必要这样
        //为mtpye赋值
        snd_msg.msgInfo.msg_id=i;
        msgsnd(msgqid, &snd_msg, sizeof(snd_msg.msgInfo), 0); // 发送消息 msg 到 msgmtpye 消息队列
        printf("(CLIENT %d) sent message %d\n", getpid(), i);
        msgrcv(msgqid, &rcv_msg, sizeof(rcv_msg.msgInfo), arg, 0); // 接收消息
        s_pid=rcv_msg.msgInfo.sender_pid;
        printf("(CLIENT %d) received return message %d from SERVER%d\n", getpid(), rcv_msg.msgInfo.msg_id, s_pid);
   }
   return 0;
}

```

# 运行结果

以下是我编译并运行程序得到的结果，由输出可见我们达到了要求

```shell
juanitahowe@JuanitadeMacBook-Air ipcs #optional
juanitahowe@JuanitadeMacBook-Air ipcrm -q <exist message queue id> #optional
juanitahowe@JuanitadeMacBook-Air lab02 % gcc msg_queue.c
juanitahowe@JuanitadeMacBook-Air lab02 % ./a.out        
server is called,my pid is 70015
server is looping and counter = 10
client is called, my pmtpye is 70017, my arg is 2
(CLIENT 70017) sent message 10
client is called, my pmtpye is 70016, my arg is 1
(CLIENT 70016) sent message 10
(SERVER 70015) received message 10 from CLIENT 70016
(SERVER 70015) received message 10 from CLIENT 70017
(SERVER 70015) send return message 10 to CLIENT 70016
(CLIENT 70016) received return message 10 from SERVER70015
(CLIENT 70016) sent message 9
(CLIENT 70017) received return message 10 from SERVER70015
(CLIENT 70017) sent message 9
(SERVER 70015) send return message 10 to CLIENT 70017
server is looping and counter = 9
(SERVER 70015) received message 9 from CLIENT 70016
(SERVER 70015) received message 9 from CLIENT 70017
(SERVER 70015) send return message 9 to CLIENT 70016
(SERVER 70015) send return message 9 to CLIENT 70017
server is looping and counter = 8
(CLIENT 70016) received return message 9 from SERVER70015
(CLIENT 70017) received return message 9 from SERVER70015
(CLIENT 70016) sent message 8
(CLIENT 70017) sent message 8
(SERVER 70015) received message 8 from CLIENT 70016
(SERVER 70015) received message 8 from CLIENT 70017
(SERVER 70015) send return message 8 to CLIENT 70016
(SERVER 70015) send return message 8 to CLIENT 70017
server is looping and counter = 7
(CLIENT 70016) received return message 8 from SERVER70015
(CLIENT 70016) sent message 7
(CLIENT 70017) received return message 8 from SERVER70015
(CLIENT 70017) sent message 7
(SERVER 70015) received message 7 from CLIENT 70016
(SERVER 70015) received message 7 from CLIENT 70017
(SERVER 70015) send return message 7 to CLIENT 70016
(SERVER 70015) send return message 7 to CLIENT 70017
server is looping and counter = 6
(CLIENT 70016) received return message 7 from SERVER70015
(CLIENT 70016) sent message 6
(CLIENT 70017) received return message 7 from SERVER70015
(CLIENT 70017) sent message 6
(SERVER 70015) received message 6 from CLIENT 70016
(SERVER 70015) received message 6 from CLIENT 70017
(SERVER 70015) send return message 6 to CLIENT 70016
(SERVER 70015) send return message 6 to CLIENT 70017
(CLIENT 70016) received return message 6 from SERVER70015
server is looping and counter = 5
(CLIENT 70016) sent message 5
(CLIENT 70017) received return message 6 from SERVER70015
(CLIENT 70017) sent message 5
(SERVER 70015) received message 5 from CLIENT 70016
(SERVER 70015) received message 5 from CLIENT 70017
(SERVER 70015) send return message 5 to CLIENT 70016
(SERVER 70015) send return message 5 to CLIENT 70017
server is looping and counter = 4
(CLIENT 70016) received return message 5 from SERVER70015
(CLIENT 70017) received return message 5 from SERVER70015
(CLIENT 70016) sent message 4
(CLIENT 70017) sent message 4
(SERVER 70015) received message 4 from CLIENT 70016
(SERVER 70015) received message 4 from CLIENT 70017
(SERVER 70015) send return message 4 to CLIENT 70016
(SERVER 70015) send return message 4 to CLIENT 70017
server is looping and counter = 3
(CLIENT 70017) received return message 4 from SERVER70015
(CLIENT 70016) received return message 4 from SERVER70015
(CLIENT 70017) sent message 3
(CLIENT 70016) sent message 3
(SERVER 70015) received message 3 from CLIENT 70016
(SERVER 70015) received message 3 from CLIENT 70017
(SERVER 70015) send return message 3 to CLIENT 70016
(SERVER 70015) send return message 3 to CLIENT 70017
(CLIENT 70016) received return message 3 from SERVER70015
server is looping and counter = 2
(CLIENT 70016) sent message 2
(CLIENT 70017) received return message 3 from SERVER70015
(CLIENT 70017) sent message 2
(SERVER 70015) received message 2 from CLIENT 70016
(SERVER 70015) received message 2 from CLIENT 70017
(SERVER 70015) send return message 2 to CLIENT 70016
(SERVER 70015) send return message 2 to CLIENT 70017
server is looping and counter = 1
(CLIENT 70017) received return message 2 from SERVER70015
(CLIENT 70016) received return message 2 from SERVER70015
(CLIENT 70017) sent message 1
(CLIENT 70016) sent message 1
(SERVER 70015) received message 1 from CLIENT 70016
(SERVER 70015) received message 1 from CLIENT 70017
(SERVER 70015) send return message 1 to CLIENT 70016
(SERVER 70015) send return message 1 to CLIENT 70017
(CLIENT 70016) received return message 1 from SERVER70015
(CLIENT 70017) received return message 1 from SERVER70015

```

# 总结和感想
## 关于系统调用
