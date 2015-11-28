#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <setjmp.h>

sigjmp_buf buf;
int f = 0;
void man(int s)
{
  printf("Manejador!!!\n");
  siglongjmp(buf,1);
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

  if(sigsetjmp(buf, 1) == 0)
  {
    r = g/f;

    printf("r vale %d\n", r); 
  } 
  else
  {
    printf("Error en division; recuperando...\n");
  } 

  return 0;
}

#/** PhEDIT attribute block
#-11:16777215
#0:569:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
