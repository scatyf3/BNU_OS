#include<sys/stat.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<stdio.h>


int main()
{
    int res;
    
    res=msgget(75,0755);
    
    if(res==-1)
    {
        printf("msgget is error\n");//不成功 提示 error
        return -1;
    }
    printf("msgget is sueecss ID is %d\n",res);//成功返回 id
    
      return 0;

}

