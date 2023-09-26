#include <stdlib.h> 
#include <signal.h>
#include <stdio.h>
int p1,p2; 

main() 
{
	int fd[2];
	char OutPipe[100],InPipe[100];
	pipe(fd);               /*�����̽����ܵ�*/
	while ((p1=fork())==-1);    /*�����ӽ���p1,ʧ��ʱѭ��*/
	if (p1==0)                   /*ִ���ӽ���p1*/
	{
		lockf(fd[1],1,0);      /*��������д���*/
		sprintf(OutPipe,"child 1 process is sending message!");
		write(fd[1],OutPipe,50);   /*��OutPipe�е�50���ַ�д��ܵ�*/
		sleep(5);                 /*˯��5��*/
		lockf(fd[1],0,0);         /*�ͷ�д���*/
		exit(0);                  /*�ر�pid1*/
	}
	else /*�Ӹ����̷��أ�ִ�и�����*/
	{
		while ((p2=fork())==-1);   /*�����ӽ���p2,ʧ��ʱѭ��*/
		if (p2==0)                  /*���ӽ���p2���أ�ִ���ӽ���pid2*/
		{
			lockf(fd[1],1,0);     /*��������д���*/
			sprintf(OutPipe,"child 2 process is sending message!");
			write(fd[1],OutPipe,50);   /*��OutPipe�е�50���ַ�д��ܵ�*/
			sleep(5);                  /*˯��5��*/
			lockf(fd[1],0,0);          /*�ͷ�д���*/
			exit(0);                   /*�ر�pid2*/
		}
		else
		{
			read(fd[0],InPipe,50);   /*��ȡ�ܵ��е���Ϣ*/
			printf("%s\n",InPipe);   /*��ӡ��Ϣ*/
			read(fd[0],InPipe,50);
			printf("%s\n",InPipe);
			
		}

	}
}