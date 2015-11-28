#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <signal.h>
#include <errno.h>

int cnt = 0;

void m(int s)
{
  cnt++;
}

void m1(int s, siginfo_t *info, void *p)
{
  cnt++;
}

int main(int argc, char **argv)
{
  sigset_t masc;
  struct sigaction sa;
  int i;
  struct timespec tim = {1, 0};
  siginfo_t info;
  union sigval valor;
    
  sigemptyset(&masc);
  sigaddset(&masc, SIGRTMIN);

  /* SIGRTMIN NO se programa para tiempo real */

  sa.sa_handler = m;  
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGRTMIN, &sa, NULL);

  printf("bloqueando senales\n");
  sigprocmask(SIG_BLOCK, &masc, NULL);
 
  for(i=0; i<10; i++)
    kill(getpid(), SIGRTMIN);

  sleep(2);
  
  printf("desbloqueando senales\n");
  sigprocmask(SIG_UNBLOCK, &masc, NULL);


  while(sleep(2) > 0);

  printf("han llegado %d senales\n", cnt);

  /* Ahora si se programa para tiempo real */

  sa.sa_sigaction = m1;  
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGRTMIN, &sa, NULL);

  cnt = 0;  

  printf("bloqueando senales\n");
  sigprocmask(SIG_BLOCK, &masc, NULL);
 
  for(i=0; i<10; i++)
    kill(getpid(), SIGRTMIN);

  sleep(2);
  
  printf("desbloqueando senales\n");
  sigprocmask(SIG_UNBLOCK, &masc, NULL);


  while(sleep(2) > 0); /* Para que lleguen todas antes del printf */
 
  printf("han llegado %d senales\n", cnt);

  /* Ahora se envian datos con sigqueue y se
     realiza una espera sincrona */

  cnt = 0;  

  printf("bloqueando senales\n");
  sigprocmask(SIG_BLOCK, &masc, NULL);
 
  for(i=0; i<10; i++)
  {
    valor.sival_int = i;
    sigqueue(getpid(), SIGRTMIN, valor);
  }
    
  printf("recogiendo senales\n");

  /* Cuando salga con -1, entonces es que ha saltado el
     sobretiempo, y no quedan mas senales que reconocer */

  while(sigtimedwait(&masc, &info, &tim) >= 0) 
   printf("dato %d\n", info.si_value.sival_int);


}


#/** PhEDIT attribute block
#-11:16777215
#0:1891:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000116)**/
