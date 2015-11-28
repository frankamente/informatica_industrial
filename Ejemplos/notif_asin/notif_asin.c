/* Prueba de notificacion asincrona */

#define _POSIX_C_SOURCE 199309L

#include <unistd.h>
#include <mqueue.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>

#define TAM_MSG 32
#define DIM_COLA 8

int cuenta_msg = 0;
void enviador(char *ncola);

/* Manejador para la senal de notificacion asincrona */
/* Las funciones relacionadas con la cola de mensajes no son
    async-safe, pero vamos a impedir que sean invocadas
    al mismo tiempo desde main */
    
void manejador(int sig, siginfo_t *datos, void *aux)
{
     mqd_t *pcola;
     struct mq_attr attr;
     struct sigevent evento;
     union sigval valor;
     char buffer[TAM_MSG];
     int pri;
     int nmsg;
                    
     pcola = datos->si_value.sival_ptr;
     valor = datos->si_value;
     
     /* Reponer notificacion asincrona */
     
     evento.sigev_notify = SIGEV_SIGNAL;
     evento.sigev_signo = SIGRTMIN;
     evento.sigev_value = valor;
     mq_notify(*pcola, &evento);
     
     /* Vaciar la cola */

     do
     {
        mq_receive(*pcola, buffer, TAM_MSG, &pri);
        cuenta_msg++;
        mq_getattr(*pcola, &attr);
        nmsg = attr.mq_curmsgs;
     } while(nmsg != 0);     
}

/* Manejador para la senal de notificacion asincrona */
/* En este caso no vacia la cola */

void manejador1(int sig, siginfo_t *datos, void *aux)
{
     mqd_t *pcola;
     struct sigevent evento;
     union sigval valor;
     char buffer[TAM_MSG];
     int pri;
          
     pcola = datos->si_value.sival_ptr;
     valor = datos->si_value;
     
     /* Reponer notificacion asincrona */
     
     evento.sigev_notify = SIGEV_SIGNAL;
     evento.sigev_signo = SIGRTMIN;
     evento.sigev_value = valor;
     mq_notify(*pcola, &evento);
     
     mq_receive(*pcola, buffer, TAM_MSG, &pri);
     cuenta_msg++; 
 }
 
 /* Programa principal */
 
 int main(int argc, char **argv)
 {
     char ncola[] = "/prueba_notif";
     mqd_t icola;
     struct mq_attr attr;
     pid_t pid_enviador;
     sigset_t sig_notif;
     struct sigaction acc;
     int i;
     struct sigevent evento;
     struct timespec t2sg = {2, 0};
     struct timespec tim;
     int pri;
     char buffer[TAM_MSG];
                              
     /* Crear cola */
     
     mq_unlink(ncola);
     attr.mq_maxmsg = DIM_COLA;
     attr.mq_msgsize = TAM_MSG;
     attr.mq_flags = 0;
     icola = mq_open(ncola, O_CREAT | O_RDONLY | O_EXCL, S_IRWXU, &attr);

     /* Preparar senal */
     
     sigemptyset(&sig_notif);
     sigaddset(&sig_notif, SIGRTMIN);
     sigprocmask(SIG_BLOCK, &sig_notif, NULL);
     
     acc.sa_flags = SA_SIGINFO;
     acc.sa_sigaction = manejador;
     sigemptyset(&acc.sa_mask);
     sigaction(SIGRTMIN, &acc, NULL);

     /* Activar notificacion */
     /* Como dato, un puntero al identificador de cola */
          
     evento.sigev_notify = SIGEV_SIGNAL;
     evento.sigev_signo = SIGRTMIN;
     evento.sigev_value.sival_ptr = &icola;
     mq_notify(icola, &evento);
                              
     /* Crear enviador */
     
     pid_enviador = fork();
     if(pid_enviador == 0)
     {
         enviador(ncola);
         exit(1);
     }               
     
     /* Los mensajes se acumulan durante 2 segundos */

     tim = t2sg;     
     nanosleep(&tim, NULL);
     sigprocmask(SIG_UNBLOCK, &sig_notif, NULL);
     
     printf("Recibidos %d mensajes acumulados durante 2 segundos\n", cuenta_msg);     
     
     /* Ahora entran libremente */

     cuenta_msg = 0;
     tim = t2sg;
     while(nanosleep(&tim, &tim) == -1);
     printf("Recibidos %d mensajes que entran libremente durante 2 segundos\n", cuenta_msg);     
     
     /* Programamos el otro manejador */

     acc.sa_sigaction = manejador1;
     sigaction(SIGRTMIN, &acc, NULL);
 
     printf("Se ha cambiado el manejador a manejador1, que no vacia la cola\n");
     
     /* Los mensajes siguen entrando libremente */
     
     cuenta_msg = 0;
     tim = t2sg;
     while(nanosleep(&tim, &tim) == -1);
     printf("Recibidos %d mensajes que entran libremente durante 2 segundos\n", cuenta_msg);     
             
     /* Ahora se acumularan de nuevo durante 2 segundos */

     sigprocmask(SIG_BLOCK, &sig_notif, NULL);
     
     tim = t2sg;     
     cuenta_msg  = 0;
     nanosleep(&tim, NULL);
     sigprocmask(SIG_UNBLOCK, &sig_notif, NULL);
     
     printf("Recibidos %d mensajes acumulados durante 2 segundos\n", cuenta_msg);     

     /* Ahora entraran libremente (o entrarian, si hubiera senales) */

     cuenta_msg = 0;
     tim = t2sg;
     while(nanosleep(&tim, &tim) == -1);
     printf("Recibidos %d mensajes que entran libremente durante 2 segundos\n", cuenta_msg);     
     
     /* Ahora vaciamos la cola  */

     sigprocmask(SIG_BLOCK, &sig_notif, NULL);
     
     i  = 0;
     do
     {
        mq_receive(icola, buffer, TAM_MSG, &pri);
        mq_getattr(icola, &attr);
        i++;
     } while(attr.mq_curmsgs != 0);

     printf("Leidos %d mensajes atascados\n", i);
     
     /* Ahora entraran de nuevo libremente */

     sigprocmask(SIG_UNBLOCK, &sig_notif, NULL);

     cuenta_msg = 0;
     tim = t2sg;
     while(nanosleep(&tim, &tim) == -1);
     printf("Recibidos %d mensajes que entran libremente durante 2 segundos\n", cuenta_msg);     
     
           
     /* Terminando */
          
     kill(pid_enviador, SIGTERM);
     wait(&i);
 }

 /* Funcion para el proceso de envio de mensajes */
  
 void enviador(char *ncola)
 {
     mqd_t ocola;
     struct timespec tim = {0, 200000000L};
     int cnt;
     char buffer[TAM_MSG];
     
     ocola = mq_open(ncola, O_WRONLY, 0, NULL);
     
     /* Envio de mensajes */
     
     while(1)
     {
         buffer[0] = cnt++;
         mq_send(ocola, buffer, TAM_MSG, 0);
         nanosleep(&tim, NULL);
     }
     
     exit(0);
 }


