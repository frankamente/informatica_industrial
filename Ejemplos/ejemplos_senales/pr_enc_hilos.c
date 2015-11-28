/* Prueba de encolamiento de senales cuando se usan con hilos */

#define _POSIX_C_SOURCE 199506

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

void *rhilo(void *p);

void action(int sig, siginfo_t *info, void *p)
{
  printf("action!\n");
}

int main()
{
  pthread_t hilo;
  pthread_t hilo1;
  int i;
  sigset_t set;
  struct sigaction acc;
  union sigval dato;

  sigemptyset(&set);
  sigaddset(&set, SIGRTMIN);
  pthread_sigmask(SIG_BLOCK, &set, NULL);

  acc.sa_sigaction = action;
  acc.sa_flags = SA_SIGINFO;
  sigemptyset(&acc.sa_mask);
  sigaction(SIGRTMIN, &acc, NULL);

  pthread_create(&hilo, NULL, rhilo, (void *)0);
  pthread_create(&hilo1, NULL, rhilo, (void *)1);

  printf("main enviando 10 senales\n");
  for(i=0; i<10; i++)
  {
   pthread_kill(hilo, SIGRTMIN);
  }

  for(i=0; i<10; i++)
  {
   pthread_kill(hilo1, SIGRTMIN);
  }
  printf("Senales enviadas\n");
  sleep(5);
  pthread_cancel(hilo);
  pthread_cancel(hilo1);
  pthread_join(hilo, NULL);
/*
  pthread_create(&hilo, NULL, rhilo, (void *)0);
  pthread_create(&hilo1, NULL, rhilo, (void *)1);
*/
  printf("main enviando 10 senales\n");
  for(i=0; i<10; i++)
  {
    dato.sival_int = i;
    sigqueue(getpid(), SIGRTMIN, dato); 
  }

  pthread_create(&hilo, NULL, rhilo, (void *)0);
  pthread_create(&hilo1, NULL, rhilo, (void *)1);

  sleep(5);
  pthread_cancel(hilo);
  pthread_cancel(hilo1);
  pthread_join(hilo, NULL);

  return 0;
}

void *rhilo(void *p)
{
  int res;
  sigset_t set;
  siginfo_t info;
  int cnt;
  struct timespec t = {0, 100000000L};

  sigemptyset(&set);
  sigaddset(&set, SIGRTMIN);
  sleep(2); 

   printf("rhilo %d tomando senales\n", (int)p);
   cnt = 0;
   while(1)
   {
      res = sigwaitinfo(&set, &info);
      if(res == -1) printf("rhilo %d: error en sigwaitinfo %s\n", (int)p, strerror(errno));
      else
      {
         cnt++;
         printf("rhilo %d: Recibida senal %d, cnt %d ", (int)p, res, cnt);
         if(info.si_code == SI_QUEUE) printf("con dato %d\n", info.si_value.sival_int);
         else printf("\n");
      }
      nanosleep(&t, NULL); 
   } 
   printf("rhilo: recibidas %d senales\n", cnt);
}
