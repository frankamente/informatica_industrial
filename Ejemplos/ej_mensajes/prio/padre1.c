/* Prueba de la prioridad de mensajes */

#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h> 


int main(int argc, char **argv)
{
  int i;
  int pid;
  int estado;
  mqd_t idcola;
  struct mq_attr atributos;
  int buf[10] = {0,0,0,0,0,0,0,0,0,0};
  int retorno;
  struct timespec ret = {0, 200000000L};

  printf("padre -> Hola mundo\n");

  /* Creando la cola de mensajes */

  atributos.mq_maxmsg = 100;         /* 100 mensajes */
  atributos.mq_msgsize = sizeof(buf);/* Los mensajes son tan grandes como buf */
  atributos.mq_flags = 0;
   
  idcola = mq_open("/cola1", O_CREAT | O_EXCL | O_RDWR, S_IRWXU, &atributos);
  if(idcola == (mqd_t)-1)
  {
    if(errno == EEXIST)
    {
      printf("La cola existia\n");
      mq_unlink("/cola1");
      printf("cola borrada\n");
      idcola = 
        mq_open("/cola1", O_CREAT | O_EXCL | O_RDWR, S_IRWXU, &atributos);
    }
    else
    {
      printf("Error irrecuperable al abrir la cola\n");
      exit(1);
    }        
  }

  /* Arrancando hijo (receptor) */

  pid = fork();
  if(!pid)
  {
    printf("hijo -> soy el hijo %d\n", pid);
    if(execl("./hijo1", "hijo", "/cola1", NULL) < 0) 
    {
      printf("... pero hay algun problema. Adios!\n");
      exit(1);
    } 
  }


 for(i=0; i<64; i++)
 {
   /* En la primera mensaje del mensaje hay una marca de identificacion */

   /*nanosleep(&ret, NULL);  */

   buf[0] = i;

   /* Los impares son de baja prioridad, y los pares de alta prioridad */

   printf("padre -> envio %d\n", i);
   if(i%2) retorno = mq_send(idcola, (char *)buf, sizeof(buf), 1);
   else    retorno = mq_send(idcola, (char *)buf, sizeof(buf), 2);

   if(retorno < 0) 
   { 
     printf("padre -> error\n");
     if(errno == EBADF) fprintf(stderr, "no me vale el descriptor\n");
     if(errno == EINVAL) fprintf(stderr, "no vale la prioridad\n");
     if(errno == EMSGSIZE) fprintf(stderr, "buffer dem. grande\n");
     if(errno == EAGAIN) fprintf(stderr, "no hay sitio\n");
     if(errno == EINTR) fprintf(stderr, "llega una segnal\n");
   }
 }

 /* Mensaje de prioridad 0: Indica final de transmision */ 

 retorno = mq_send(idcola, (char *)buf, sizeof(buf), 0);

 /* Cerrando y borrando la cola */

 mq_close(idcola);
 mq_unlink("/cola1");

 printf("padre -> acabando\n");
  
}
