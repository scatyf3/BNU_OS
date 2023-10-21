#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
   pid_t pid1, pid2;
   while ((pid1 = fork()) == -1);
   if (pid1 == 0)
   {
      lockf(1, 1, 0);
      printf("pid1 here : b\n");
      lockf(1, 0, 0);
   }
   else
   {
      while ((pid2 = fork()) == -1);
      if (pid2 == 0)
      {
         lockf(1, 1, 0);
         printf("pid2 here : c\n");
         lockf(1, 0, 0);
      }
      else
      {
         lockf(1, 1, 0);
         printf("father progress here : a\n");
         lockf(1, 0, 0);
      }
   }
   return 0;
}