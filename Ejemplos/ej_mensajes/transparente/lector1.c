#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <mqueue.h>
#include <errno.h>


int main(int argc, char **argv)
{
  mqd_t micola;
  char *buf;      /* Puntero a los datos recibidos */
  int result;
  int prio;
  struct mq_attr atris;
  int i;
  
  fprintf(stderr, "lector -> activado\n", argv[0]);

  /* Apertura de la cola: Se reintenta tantas veces como sea preciso */

  do
  {
    micola = mq_open("/unacola", O_RDWR, 0, NULL);
    if(micola == (mqd_t)-1) 
    {
      fprintf(stderr, "lector -> error al abrir; probaremos de nuevo\n");
      sleep(1);
    }
  } while(micola == (mqd_t)-1);

  fprintf(stderr, "lector -> por fin consegui abrir la cola\n");

  /* Se dimensiona el buffer de datos al tamano del mensaje */

  mq_getattr(micola, &atris);
  buf = (char *)malloc(atris.mq_msgsize);

  /* Bucle de recepcion: Todo lo que llega es enviado por la salida
     standard hasta encontrar un "\0" */

  while(1)
  {
    result = mq_receive(micola, (char *)buf, atris.mq_msgsize, &prio); 
    if(*buf == '\0') break;

    for(i=0; i<atris.mq_msgsize; i++)
      putchar(buf[i]);
    fflush(stdout);
    fprintf(stderr, "\ntexto volcado a salida\n");
  }

  /* Final */

  mq_close(micola);
  fprintf(stderr, "lector -> acabando\n");
}
