#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
int f = 0;
void man(int s)
{
  printf("Manejador!!!\n");
  exit(2);  
}

int main(int argc, char **argv)
{
  int f = 0;
  int g = 4;
  int r;
  sigset_t conj;
  struct sigaction acc;

  acc.sa_flags = 0;
  acc.sa_handler = man;
  sigemptyset(&acc.sa_mask);
  sigaction(SIGFPE, &acc, NULL);

  sigemptyset(&conj);
  sigaddset(&conj, SIGFPE);
/*
  sigfillset(&conj); 
  sigprocmask(SIG_BLOCK, &conj, NULL); 
*/

  r = g/f;

  printf("r vale %d\n", r); 

  return 0;
}

#/** PhEDIT attribute block
#-11:16777215
#0:569:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
