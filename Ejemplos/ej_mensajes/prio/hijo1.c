#define POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/stat.h>


int main(int argc, char **argv)
{
  mqd_t idcola;
  int *buf;
  int result;
  int prio;
  int nfallos = 0;
  int npdos = 0;
  int npuno = 0;
  struct mq_attr atrib;
  struct timespec tim = {0, 10000L};   
  
  printf("programa %s usando cola %s\n", argv[0], argv[1]);

  /* Apertura de la cola */

  idcola = mq_open(argv[1], O_RDWR, 0, NULL);

  if(idcola == (mqd_t)-1)
  {
     printf("hijo -> error al abrir\n");
     exit(1);
  }

  mq_getattr(idcola, &atrib);
  buf = (int *)malloc(atrib.mq_msgsize);

  prio = -1;  /* Inicialmente: Prioridad no valida */

  printf("hijo -> empezando a leer\n");

  /* Termina al detectar un mensaje de prioridad 0 */

  while(prio != 0)
  {
    result = mq_receive(idcola, (char *)buf, atrib.mq_msgsize, &prio); 
    if(result != -1)
    {
      printf("hijo -> recibido %d con pri. %d \n", buf[0], prio);
      if(prio == 1) npuno++;
      if(prio == 2) npdos++;
    }
    else 
    { 
      nfallos++;
      printf("hijo -> fallo\n");
    }

    /* Retraso para que se acumulen los mensajes */

    nanosleep(&tim, NULL);  
  }

  mq_close(idcola);

  printf( 
          "hijo -> acabando; %d total recib.. %d pr1, %d pr2, %d fallos\n", 
           (npuno+npdos), npuno, npdos, nfallos);
}
