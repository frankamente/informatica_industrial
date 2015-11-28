#define _POSIX_C_SOURCE 199506L
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>

void man(int sig)
{
   int i;
   for(i=0; i<5; i++)
   {
      printf("Manejador cuenta %d en hilo %d\n", i, (int)pthread_self());
      sleep(1);
   }
}

void *rhilo(void *p)
{
   int id = (int)p;
   struct timespec t = {1, 0};
   
   while(1)
   {
      printf("Soy hilo %d con id %d\n", id, (int)pthread_self());
    /*  sleep(1); */if(nanosleep(&t, NULL) == -1) printf("hilo %d interrumpido\n", id); 
   }
   return NULL;
}


int main(int argc, char **argv)
{
  pthread_t th1, th2;
  sigset_t sig;
  struct sigaction sa;
  int i;

  sigemptyset(&sig);
  sigaddset(&sig, SIGRTMIN);
  pthread_sigmask(SIG_BLOCK, &sig, NULL);

  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = man;
  sigaction(SIGRTMIN, &sa, NULL);

  pthread_create(&th1, NULL, rhilo, (void *)0);

  pthread_sigmask(SIG_UNBLOCK, &sig, NULL);
  pthread_create(&th2, NULL, rhilo, (void *)1);
  pthread_sigmask(SIG_BLOCK, &sig, NULL);
  kill(getpid(), SIGRTMIN); 

  for(i=0; i<10; i++)
  {
     printf("main contando %d\n", i);
     sleep(1);
  }
  printf("main acabando...\n");

  return 0;
}
