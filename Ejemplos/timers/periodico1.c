/* Pruebas de temporizadores */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <errno.h>
#include <time.h>

#define NTST  10 
#define NTST1 50

/* Manejador, que no se utiliza */

void manejador(int signo, siginfo_t *datos, void *pa_na)
{
  printf("segnal llega al manejador!!!\n");
}

/* Tiempo transcurrido en segundos */

double lapso(struct timespec antes, struct timespec ahora)
{
  return (double)(ahora.tv_sec - antes.tv_sec) +
         ((double)(ahora.tv_nsec - antes.tv_nsec))/1000000000.;
}

/* Programa principal */

int main(int argc, char **argv)
{
  sigset_t la_de_tim;
  struct sigaction accion;
  int i;
  int over;
  struct timespec interv = { 1, 0};           /* Un segundo */
  struct timespec parada = { 10, 500000000L}; /* 10.5 segundos */
  struct timespec doscmseg = { 0, 200000000L}; /* 200 milisegundos */
  struct timespec cero = { 0, 0};
  struct timespec ahora;
  struct timespec antes;
  struct timespec inicial;
  struct timespec resolu;
  struct itimerspec tempo;
  int res;
  float media;
  float intrv;

  timer_t  mitimer;
  struct sigevent evento;
  siginfo_t info;

  sigset_t launo;

  /* Observando caracteristicas del reloj */

  clock_getres(CLOCK_REALTIME, &resolu);

  printf("El reloj tiene resolucion de %ld seg %ld nanoseg\n",
          (long)resolu.tv_sec, (long)resolu.tv_nsec);

  tempo.it_value = interv;
  tempo.it_interval = cero;

  /* Le asocio la segnal SIGRTMIN */

  /* 1: La programo de tiempo real */

  sigemptyset(&accion.sa_mask);
  accion.sa_flags = SA_SIGINFO;
  accion.sa_sigaction = manejador; 
  sigaction(SIGRTMIN, &accion, NULL);

  /* 2: La bloqueo en la mascara de proceso */

  sigemptyset(&la_de_tim);
  sigaddset(&la_de_tim, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &la_de_tim, NULL);

  /* 3: Preparo sigevent para ponerla como evento de temporizador */

  evento.sigev_signo = SIGRTMIN;
  evento.sigev_notify = SIGEV_SIGNAL;
  evento.sigev_value.sival_int = 77;

  /* Creo el temporizador */


  tempo.it_interval = doscmseg;

  
  timer_create(CLOCK_REALTIME, &evento, &mitimer); 

  clock_gettime(CLOCK_REALTIME, &antes);
  inicial = antes;
  timer_settime(mitimer, 0, &tempo, NULL); 

  timer_gettime(mitimer, &tempo); 
  printf("Valor programado: %ld sg % ld nsg\n",
          (long)tempo.it_interval.tv_sec,
          tempo.it_interval.tv_nsec);  


  nanosleep(&doscmseg, NULL);
  timer_gettime(mitimer, &tempo); 
  printf("Proxima senal %f\n", tempo.it_value.tv_nsec/1000000000.);

  nanosleep(&doscmseg, NULL);
  timer_gettime(mitimer, &tempo); 
  printf("Proxima senal %f\n", tempo.it_value.tv_nsec/1000000000.);

  /* Ahora se esperan las senales generadas y se mide el intervalo */


  media = 0.;


  for(i=0; i<NTST1; i++)
  {
    int res;
    res = sigwaitinfo(&la_de_tim, &info);
    if(res == -1) printf("Error en sigwaitinfo %s\n", strerror(errno));
    clock_gettime(CLOCK_REALTIME, &ahora);
    over = timer_getoverrun(mitimer);
    intrv = lapso(antes, ahora);

   /* Trazadores */

   
      if(over > 0) printf("Overrun de %d!\n", over);
      if(i>0) media += intrv;
      printf("Intervalo %f seg. Total de %f seg. Desv. desde el com.: %f\n", 
                   intrv,
                   lapso(inicial, ahora),
                   lapso(inicial, ahora) - 0.2*(i)-1);
   

    antes = ahora;
  }

  printf("Media de intervalos %f\n", media/(NTST1-1));

  timer_delete(mitimer);
}
