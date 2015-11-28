#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

int sigquit = 0;

void man(int signo)
{
  printf("llega senal %d\n", signo);
  switch(signo)
  {
    case SIGINT:
      printf("es sigint\n");
    break;
    case SIGQUIT:
       sigquit = 1;      
      printf("es sigquit\n");
    break;
  }
}

int main(int argc, char **argv)
{
  struct sigaction sa;
  sigset_t masc;
  
  sa.sa_handler = man;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);
  sigaction(SIGQUIT, &sa, NULL);


  sigemptyset(&masc);
  sigaddset(&masc, SIGINT);
  sigaddset(&masc, SIGQUIT);
  sigprocmask(SIG_UNBLOCK, &masc, NULL);

 
  while(1)
  {
    sleep(1);
    printf("still waiting\n");

#ifdef KKK
    if(sigquit)
    {
        sigquit = 0;
       /* printf("entro sigquit\n"); */
        sigprocmask(SIG_UNBLOCK, &masc, NULL);
    }
#endif
  }
}

#/** PhEDIT attribute block
#-11:16777215
#0:584:TextFont14:-3:-3:0
#584:612:TextFont14:0:-1:0
#612:860:TextFont14:-3:-3:0
#860:878:default:-3:-3:0
#**  PhEDIT attribute block ends (-0000195)**/
