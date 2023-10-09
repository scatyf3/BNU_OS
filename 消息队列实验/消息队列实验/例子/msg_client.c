#include <stdio.h> 
#include <sys/msg.h> 
#define MSGKEY 75         /*????????*/

struct msgform
{
	long mtype;
	char mtext[256]; 
};

main()
{
	struct msgform msg;
	int msgqid,pid,*pint;

	if ((msgqid=msgget(MSGKEY,0777))==-1)     /*??????????????*/
	{
		printf("This message queue does not exist. \n",*pint);
		return;
	}
	pid=getpid();
	pint=(int *)msg.mtext;
	*pint=pid;
	msg.mtype=1;                     /*??????????*/
	msgsnd(msgqid,&msg,sizeof(int),0);       /*??msgqid???????msg*/
	msgrcv(msgqid,&msg,256,pid,0);        /*?????????????????????*/
	printf("client:receive from pid %d\n",*pint);
}

