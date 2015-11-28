#define _POSIX_C_SOURCE 199306L

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void man(int sig, siginfo_t *pinfo, void *nulo)
{
  printf("Entra manejador con senal %d\n", sig);
  sleep(1);
}

int main(int argc, char **argv)
{
  sigset_t mascara;
  struct sigaction acc;
  int i;
  int j;
  union sigval valor;

  sigemptyset(&mascara);

  printf("SIGRTMIN vale %d, y SIGRTMAX %d\n",
       SIGRTMIN, SIGRTMAX);

  /* Prueba enmascarando las senales menos prioritarias
     en el manejador */

  for(i=SIGRTMIN; i<=SIGRTMIN+5; i++)
  {
    acc.sa_sigaction = man;
    sigemptyset(&acc.sa_mask);
    for(j=i+1; j<=SIGRTMIN+5; j++)
      sigaddset(&acc.sa_mask, j);
    acc.sa_flags = SA_SIGINFO;
    sigaction(i, &acc, NULL);
    sigaddset(&mascara, i);
  }

  sigprocmask(SIG_BLOCK, &mascara, NULL);
  for(i=SIGRTMIN+5; i>=SIGRTMIN; i--)
  {
   valor.sival_int = i;
   sigqueue(getpid(), i, valor);
  }

  sigprocmask(SIG_UNBLOCK, &mascara, NULL);

  while(sleep(2) != 0);

  printf("Fin de la primera prueba\n");

  /* Prueba sin usar la mascara del manejador */

  for(i=SIGRTMIN; i<=SIGRTMIN+5; i++)
  {
    acc.sa_sigaction = man;
    sigemptyset(&acc.sa_mask);
    acc.sa_flags = SA_SIGINFO;
    sigaction(i, &acc, NULL);
  }

  sigprocmask(SIG_BLOCK, &mascara, NULL);
  for(i=SIGRTMIN+5; i>=SIGRTMIN; i--)
  {
   valor.sival_int = i;
   sigqueue(getpid(), i, valor);
  }
  sigprocmask(SIG_UNBLOCK, &mascara, NULL);

  while(sleep(2) != 0);

  printf("Fin de la segunda prueba\n");
  return 0;
}

