#define _POSIX_C_SOURCE 199506L

#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdio.h>

volatile int aviso = 0;
int copia_i;

void manej(int senal)
{
  aviso = 1;
  printf("entra el manejador %d\n", copia_i);
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
  }
  printf("resultado %d\n", WEXITSTATUS(i));

  aviso = 0;
  alarm(1);
  
  while(!aviso)
  {
    for(i=0; i<100; i++)
    {
      copia_i = i;
      sigprocmask(SIG_BLOCK, &alarma, NULL);
      printf("numero %d avisa %d\n", i, aviso); 
      sigprocmask(SIG_UNBLOCK, &alarma, NULL);
    }
    if(aviso) printf("aviso detectado\n");
  }
}


#/** PhEDIT attribute block
#-11:16777215
#0:896:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
