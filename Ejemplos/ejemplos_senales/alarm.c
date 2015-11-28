#define _POSIX_C_SOURCE 199506L

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int aviso = 0;

void manej(int senal)
{
  aviso = 1;
  printf("entra el manejador\n");
/*  fflush(stdout); */
}

int main(int argc, char **argv)
{
  sigset_t alarma;
  struct sigaction acc;
  int i;
  pid_t proc;

  sigemptyset(&alarma);
  sigaddset(&alarma, SIGALRM);


  acc.sa_flags = 0;
  sigemptyset(&acc.sa_mask);
  acc.sa_handler = manej;
  
  sigaction(SIGALRM, &acc, NULL);

  sigprocmask(SIG_UNBLOCK, &alarma, NULL);

  
  proc = fork();
  if(proc == 0)
  {
    sleep(5);
    exit(5);
  }

    
  alarm(2);
  while(wait(&i) == -1)
  {
    printf("error %s\n", strerror(errno));
    fflush(stdout);
  }
  printf("resultado %d\n", WEXITSTATUS(i));
  fflush(stdout);

  aviso = 0;
  alarm(1);
  
  while(!aviso)
  {
    for(i=0; i<100; i++)
    { 
      sigprocmask(SIG_BLOCK, &alarma, NULL); 
      printf("numero %d\n", i); 
      sigprocmask(SIG_UNBLOCK, &alarma, NULL); 
    }
    if(aviso) printf("aviso detectado\n");
  }
  return 0;
}


#/** PhEDIT attribute block
#-11:16777215
#0:1072:TextFont12:-3:-3:0
#**  PhEDIT attribute block ends (-0000116)**/
