/* Proceso padre: prueba de envio de senales de tiempo real */

#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h> 
#include <time.h>

int main(int argc, char **argv)
{
  int pid;
  int i;
  union sigval valor;
  struct timespec tim = {0, 500000000L};
 
  fprintf(stderr, "padre -> Hola mundo.\n");
  fprintf(stderr, "Numero de senales de T.R. segun sysconf: %ld\n",
          sysconf(_SC_RTSIG_MAX));
  fprintf(stderr, "Longitud de las colas, segun sysconf: %ld\n",
          sysconf(_SC_SIGQUEUE_MAX));

/*  sigemptyset(&set); sigaddset(&set, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &set, NULL); */

  /* Activacion del hijo */

  pid = fork();
  if(!pid)
  {
    fprintf(stderr, "hijo -> soy el hijo %d\n", pid);
    execl("hijo1", "hijo", NULL); 
    fprintf(stderr, "hijo -> algo ha ido mal\n");
    exit(1);
  }

  sleep(1); /* Espera para permitir que el hijo prepare las senales */

  /* Bucle de envio de senales con dato incluido */

  fprintf(stderr, "padre-> Empiezo a enviar senales!\n");

  for(i=0; i<10; i++)
  {
    valor.sival_int = i; 
    if(sigqueue(pid, SIGRTMIN, valor) == -1) 
      fprintf(stderr, "padre -> no puedo encolar la senal\n"); 
    nanosleep(&tim, NULL); 
  } 

  fprintf(stderr, "padre ->  acabando.\n");  
  return 0;
}


#/** PhEDIT attribute block
#-11:16777215
#0:1404:TextFont12:-3:-3:0
#**  PhEDIT attribute block ends (-0000116)**/
