#include<stdio.h> 
#include<stdlib.h> 
#include <sys/shm.h>

#define SHMKEY 75         /*���干�����ؼ���*/

int shmid,p1,p2;
int *addr;
 
void CLIENT()
{
	int i;
	shmid=shmget(SHMKEY,1024,0777);  /*��ȡ������������Ϊ1024���ؼ���SHMKEY*/
	addr=(int *)shmat(shmid,0,0);           /*��������ʼ��ַΪaddr*/
	for(i=9;i>=0;i--)
	{
		while(*addr!=-1);
		printf("(client)sent%d\n",i);      /*��ӡ(client)sent*/
		*addr=i;                           /*��i����addr*/
	}
	exit(0);
}

void SERVER()
{
	shmid=shmget(SHMKEY,1024,0777|IPC_CREAT);     /*����������*/
	addr=(int *)shmat(shmid,0,0);                 /*��������ʼ��ַΪaddr*/
	do
	{
		*addr=-1;
		while(*addr==-1);
		printf("(server)received %d\n",*addr);	 /*�������ʹ�ù�����*/
	}while(*addr);
	shmctl(shmid,IPC_RMID,0);
	exit(0);
}


int main()
{
	
	while ((p1=fork())==-1);                      /*�����ӽ���p1,ʧ��ʱѭ��*/
	if (!p1) SERVER();                             /*�ӽ���p1ִ��SERVER()*/
	while ((p2=fork())==-1);                      /*�����ӽ���p2,ʧ��ʱѭ��*/   
	if (!p2) CLIENT();                             /*�ӽ���p2ִ��CLIENT()*/
	wait(0);
	wait(0);
	return 0;
}