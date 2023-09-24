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
      for (int i = 0; i < 5; ++i)
         printf("pid1 here : %d\n", i);
      lockf(1, 0, 0);

   }
   else
   {
      while ((pid2 = fork()) == -1);
      if (pid2 == 0)
      {
         lockf(1, 1, 0);
         for (int i = 0; i < 5; ++i)
            printf("pid2 here : %d\n", i);
         lockf(1, 0, 0);
      }
      else printf("father progress here : a\n");
   }
   return 0;
}