#define POSIX_C_SOURCE 199309

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <time.h>

/* Manejador de la senal de terminacion */

int fin;

void handler(int signo)
{
  printf("terminando cliente\n");
  fin = 1;
}

int main(int argc, char **argv)
{
  mqd_t micola;
  char *buf;
  int result;
  struct mq_attr atributos;
  struct timespec tim = { 1,000000000L};
  unsigned int prio = 0;
  sigset_t usr1;
  struct sigaction accion;

  /* Programacion y desbloqueo de la senal SIGUSR1 */

  fin = 0;

  accion.sa_handler = handler;
  accion.sa_flags = 0;
  
  sigemptyset(&usr1);
  sigaddset(&usr1, SIGUSR1);
  sigaction(SIGUSR1, &accion, NULL);
  sigprocmask(SIG_UNBLOCK, &usr1, NULL);
  
  printf("programa %s usando cola %s\n", 
         argv[0], argv[1]);

  /* Apertura de la cola. Se supone que la cola ya existe */

  micola = mq_open(argv[1], O_RDWR, 0, NULL);
  if(micola == (mqd_t)-1) printf("error al abrir\n");
  else printf(" %s cola abierta\n", argv[0]); 

  /* Es posible identificar sus parametros */

  result = mq_getattr(micola, &atributos);
  if(result == -1) printf("error al buscar atributos\n");
  else printf(" %s atributos buscados\n", argv[0]); 

  buf = malloc(atributos.mq_msgsize);
  strcpy(buf, argv[0]);

 /* fprintf(stderr, "%s va a enviar\n", argv[0]);*/

  /* Bucle de envio de mensajes */

  while(!fin)
  { 
    result = mq_send(micola, buf, atributos.mq_msgsize, prio); 

  /*  fprintf(stderr, "%s envia con resultado %d\n", argv[0], result); */

    if(result == -1) perror(argv[0]);

    /*sleep(1);*/
    nanosleep(&tim, NULL);
  }

  mq_close(micola);

}
