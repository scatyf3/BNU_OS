#include <stdio.h> 
#include <sys/msg.h> 
#define MSGKEY 75         /*����ؼ���*/

struct msgform
{
	long mtype;
	char mtext[256]; 
};

main()
{
	struct msgform msg;
	int msgqid,pid,*pint;

	if ((msgqid=msgget(MSGKEY,0777))==-1)     /*������Ϣ����ʧ��*/
	{
		printf("This message queue does not exist. \n",*pint);
		return;
	}
	pid=getpid();
	pint=(int *)msg.mtext;
	*pint=pid;
	msg.mtype=1;                     /*ָ����Ϣ����*/
	msgsnd(msgqid,&msg,sizeof(int),0);       /*��msgqid������Ϣmsg*/
	msgrcv(msgqid,&msg,256,pid,0);        /*�������Է��������̵���Ϣ*/
	printf("client:receive from pid %d\n",*pint);
}

