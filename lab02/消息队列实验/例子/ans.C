#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#define MSGKEY 75          /*定义关键词MEGKEY*/
struct msgform                /*消息结构*/
{
    long mtype;
    char mtexe[1030];         /*文本长度*/
}msg;
int msgqid,i;
 
void CLIENT()
{
    int i;
    msgqid=msgget(MSGKEY,0777);  //创建消息队列
    for(i=10;i>=1;i--)
    {
         msg.mtype=i;
         printf("(client)sent\n");
         msgsnd(msgqid,&msg,1024,0);       //发送消息msg入msgid消息队列
    }
    exit(0);
}
 
void SERVER()
{ 
  msgqid=msgget(MSGKEY,0777|IPC_CREAT); //创建一个所有用户都可以读、写、执行的队列，因为读操作的数字代号是4，写是2，执行是1,7=4+2+1，四位数字分别代表特殊权限位，拥有者位，同组用内户位，其余用户位
  do
  {
      msgrcv(msgqid,&msg,1030,0,0);  //从队列msgid接受消息msg
      printf("(server)receive\n");
  }while(msg.mtype!=1);             //消息类型为1时，释放队列
      msgctl(msgqid, IPC_RMID,0);   //消除消息队列的标识符
      exit(0);
}
 
int main()
{
  if(fork())      //父进程
	  SERVER();
    else          //子进程
	  CLIENT();
    wait(0);
    wait(0);
}
