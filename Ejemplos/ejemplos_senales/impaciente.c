/* Programa impaciente */

#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define PACIENCIA 5

int flag;     /* Flag de comunicacion entre manejador y programa */

void manejo(int signo)
{
  flag = 1;
  printf("entra el manejador\n");
}

int main(int argc, char **argv)
{
  struct sigaction accion;
  int i;
  int caracter;
  sigset_t sigalrm;
  sigset_t anterior;

  /* Desbloqueo de SIGALRM - puede no ser necesario - */

  sigemptyset(&sigalrm);
  sigaddset(&sigalrm, SIGALRM);
  sigprocmask(SIG_UNBLOCK, &sigalrm, &anterior);

  if(!sigismember(&anterior, SIGALRM))
   printf("Pues ya estaba desbloqueada SIGALRM\n");

  /* Conexion de SIGALRM al manejador */

  accion.sa_handler = manejo;
  accion.sa_flags = 0;
  sigemptyset(&accion.sa_mask);
  sigaction(SIGALRM, &accion, NULL); 

  for(i=0; i<PACIENCIA; i++)
  {
    flag = 0;
    alarm(3);             /* Alarma 3 segundos */
    printf("otra vez->");
    caracter = getchar();
    alarm(0);             /* Desactivacion de la alarma */
    printf("\n");
    if(!flag) 
    {
      printf("bravo! has tecleado %c\n", (char)caracter);
      exit(0);
    }
    else 
    { 
      printf("has fallado de nuevo!\n");
      if(caracter == EOF) printf("recibido EOF\n");
      printf("Error %s\n", strerror(errno));
      if(errno == EINTR) printf("Es EINTR\n");
    }
  }

  printf("Se acabaron tus oportunidades\n");
  return 0;
}


#/** PhEDIT attribute block
#-11:16777215
#0:1349:TextFont14:-3:-3:0
#1349:1460:default:-3:-3:0
#**  PhEDIT attribute block ends (-0000143)**/
