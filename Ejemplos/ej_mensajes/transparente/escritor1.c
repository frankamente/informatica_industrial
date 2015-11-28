#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <signal.h>
#include <errno.h> 
#include <string.h>

#define TAM_MSG 16
#define N_MSG   20

int main(int argc, char **argv)
{
  int i;
  int car;
  mqd_t micola;
  struct mq_attr atributos;
  char buf[TAM_MSG];         /* Datos para enviar */
  int retorno;
  int fin;
  int n;


  /* Creacion y apertura de la cola de mensajes */

  atributos.mq_maxmsg = N_MSG;
  atributos.mq_msgsize = TAM_MSG;
  atributos.mq_flags = 0;
   

  /* Se borra la cola por si existiese */
 
  printf("escritor -> borrando cola\n");
  if(mq_unlink("/unacola") != -1)
    printf("Ya existia una cola\n");

  atributos.mq_maxmsg = N_MSG;
  atributos.mq_msgsize = TAM_MSG;
  atributos.mq_flags = 0;
  micola = mq_open("/unacola", O_CREAT | O_RDWR, S_IRWXU, &atributos);

  if(micola == (mqd_t)-1)
  { 
    fprintf(stderr, "escritor -> error al abrir la cola\n");
    exit(1);
  }
  else fprintf(stderr, "escritor -> cola abierta\n");

  /* Se llenan paquetes de datos con lo que se lee por la entrada 
     standard y se envian por la cola de mensajes    */
  /* Final: Cuando se encuentra final de fichero (control-D) */
 
  fin = 0;
  while(!fin)
  {
    /* Recogiendo TAM_MSG octetos */

    for(i=0; i<TAM_MSG; i++) 
    {
      car = getchar();
      if(car == EOF) 
      {
        fin = 1;
        fprintf(stderr, "\nescritor -> fin de archivo!!\n");
        break;
      }
      buf[i] = (char)car;
   }

   if(i != 0)
   {
     /* Se rellena la cola del paquete con caracteres nulos */

     for(; i<TAM_MSG; i++) buf[i] = '\0';

     fprintf(stderr, "\nescritor enviando por la cola\n");
     
     retorno = -1;
     while(retorno == -1)
     {
       retorno = mq_send(micola, (char *)buf, TAM_MSG, 0);
       if(retorno == -1) 
       {  
           fprintf(stderr, "escritor -> error en transmision %s\n", strerror(errno));
           sleep(1);
        }
       }
   }
 }

 *buf = '\0';  /* Senal de fin para el lector */
 retorno = mq_send(micola, (char*)buf, TAM_MSG, 0);

 /* La cola es cerrada y eliminada */
/*  No podria hacerse si el programa acaba anormalmente */

 mq_close(micola);
 mq_unlink("/unacola");
 printf("escritor -> acabando\n");
  
}

