#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
   pid_t id;
   char **paso;
   int i;
   
   
   if(argc < 2)
   {
     printf("Me faltan argumentos\n");
     exit(1);
   }
   else
   {
     /* argc -1 argumentos (quitando el cero) mas
        1 (el NULL que hace de terminador) */

     paso = malloc((argc-1+1)*sizeof(char *));
     for(i=1; i<argc; i++)
       paso[i-1] = argv[i];
     paso[i-1] = NULL;
         
     id = fork();
     if(id == (pid_t)0)
     {
         printf("arrancando %s con argumentos\n", argv[1]);
         for(i=0; paso[i] != NULL; i++)  printf("%s\n", paso[i]);
         execvp(argv[1], paso);
         printf("error %s en hijo\n", strerror(errno));
         exit(1);
     }
     else wait(&i);
     printf("proceso finalizado\n");
     exit(0);
  }
}
