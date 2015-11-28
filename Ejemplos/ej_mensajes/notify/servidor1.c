#define _POSIX_C_SOURCE 199309

#include <unistd.h>
#include <sys/stat.h>

#ifdef _POSIX_MESSAGE_PASSING
#include <mqueue.h>
#endif

#include <stdio.h>
#include <signal.h>
#include <errno.h> 

/* Tipos de datos */

  /* La estructura dat_proc relaciona el pid de un
     cliente con el ident. de su cola y el nombre de esta */

struct dat_proc {
		  pid_t pid;
                  mqd_t cola;
                  char *ncola;
                };

/* Prototipos de rutinas */

int prg_fin(void *man);                /* Prog. las senales de terminacion */
int prg_aviso(int senal);              /* Prog. adec. la senal de notific. */
int cliente_on(char *nexec, int n,     /* Activa cliente de nombre "nexec" */ 
               struct dat_proc *pdat); /* (el n-esimo);  alm. datos en pdat */
int espera_msg(int senal);             /* Espera algun mensaje */
int limpia(struct dat_proc *p, int nproc); /* Termina con limpieza */
void error(char *msg);                 /* Indica error */
int notif(mqd_t *pcola);               /* Repone notif. de una cola */
void manej(int);                       /* Manejador de terminacion */
void llega(int signo, siginfo_t *info, /* Manejador de notif. (no sirve) */
           void *nada);

/* Constantes simbolicas */

#define TAM_MSG  128
#define L_COLA   10
#define N_CLI    3

/* Variables globales */

 int fin;     /* Activada por el manejador de terminacion */

int main(int argc, char **argv)
{
  int alguno;
  int prio;
  int i;
  int retorno;
  char buf[TAM_MSG];            /* Buffer para el mensaje recibido */
  struct dat_proc tabla[N_CLI]; /* Registro de colas y clientes */
 
  printf("servidor -> Hola mundo\n");

  /* Preparo la segnales de terminacion */

  fin = 0;
  prg_fin(&manej);

  /* Preparo la segnal SIGRTMIN  para notificar mensajes */

  prg_aviso(SIGRTMIN);

  /* Activo clientes; incluye crear colas, porque cada cola esta 
     asociada a uno de ellos                                      */

  for(i=0; i < N_CLI; i++) cliente_on("cliente", i, tabla + i);

  /* Bucle principal */

  while(!fin)
  {
    espera_msg(SIGRTMIN);  /* Esperar senal de notificacion */

    /* En esta solucion da igual cual fuese el causante */

    alguno = 1;
    while(alguno)
    {
      alguno = 0;
      for(i=0; i<N_CLI; i++)
      {
        retorno = mq_receive(tabla[i].cola, buf, sizeof(buf), &prio);
        if(retorno != -1) 
        {
          alguno = 1;
          printf("recibido %s del %d\n", buf, i);
        }
      }
     }
   }

  /* Alguien ha enviado una senal de terminacion; el servidor termina */

  limpia(tabla, N_CLI); 

  printf("padre acabando\n");
  
}


/* Arrancar cliente, despues de crear la cola */

int cliente_on(char *nexec, int n, struct dat_proc *pdat)
{
  char an[50]; /* Se supone suficiente para albergar un entero */
  char *ncola;
  int result;
  int i;
  int pid;
  mqd_t micola;
  struct mq_attr atributos;

  /* Caracteristicas de la cola */

  atributos.mq_maxmsg = L_COLA;
  atributos.mq_msgsize = TAM_MSG;
  atributos.mq_flags = 0;
  
  /* Nombre de la cola: "/"+nexec+ascii(n) */

  sprintf(an, "%d", n);
  ncola = (char *)malloc(strlen(an)+ strlen(nexec) + 2);
  strcpy(ncola, "/");
  strcat(ncola, nexec);
  strcat(ncola, an);
 
  printf("creando cliente %d con cola %s\n", n, ncola);

  /* Abro la cola  */

  micola = mq_open(ncola, O_CREAT | O_EXCL | O_RDWR | O_NONBLOCK, 
                    S_IRWXU, &atributos);

  if(micola == (mqd_t)-1)
  {
    if(errno == EEXIST) 
    {
      printf("la cola existia\n");
      mq_unlink(ncola);
      micola = mq_open(ncola, O_CREAT |  O_EXCL | O_RDWR | O_NONBLOCK, 
               S_IRWXU, &atributos);

      if(micola == (mqd_t)-1) error("ops... error al abrir la cola (2)\n");
    }
    else error("ops... error al abrir la cola (2)\n");
  }
  else printf("cola creada y abierta\n");

 pdat->cola = micola;
 pdat->ncola = ncola;

 /* Preparando notificacion */

  notif(&pdat->cola);

 /* Activando cliente; le digo que nombre tiene la cola */

 pid = fork();
 printf("cliente %d activado\n", (int)pid);
 if(!pid)
 {
   result = execl(nexec, ncola, ncola, NULL);
   if(result == -1) printf("problema con exec\n");
   exit(1);
 }
 else pdat->pid = pid;

 return 0;
}

/* Mensaje de error */

void error(char *msg)
{
  printf("%s", msg);
  exit(1);
}

/* Manejador de terminacion */

void manej(int signo)
{
  printf("recibida segnal %d\n", signo);
  fin = 1;
}

/* Preparar notificacion con senal SIGRTMIN */
/* Como dato, se enviara un puntero al identif. de cola */

int notif(mqd_t *pcola)
{
  struct sigevent espec;

  espec.sigev_notify = SIGEV_SIGNAL;
  espec.sigev_signo = SIGRTMIN;
  espec.sigev_value.sival_ptr = (void *)pcola;
  if(mq_notify(*pcola, &espec) == -1) 
    printf("falla mq_notify\n");
}

/* Manejador de SIGRTMIN */
/* No debe entrar nunca, dada la solucion adoptada */

void llega(int signo, siginfo_t *info, void *nada)
{
  printf("ha llegado un mensaje\n");
  notif(info->si_value.sival_ptr);
}

/* Programar la terminacion del programa */

int prg_fin(void *man)
{
  struct sigaction accion;

  accion.sa_flags = 0;
  accion.sa_handler = manej;
  sigemptyset(&accion.sa_mask);
  sigaction(SIGTERM, &accion, NULL); 
  sigaction(SIGINT, &accion, NULL); 

  return 0;
}

/* Programacion de la senal de aviso de mensajes */
/* El manejador no actua, pero si que lo hace SA_SIGINFO */
/* Si no lo programo, no hay encolado de segnales */
/* Ademas hay que enmascarar la senal, para que pueda usarse sigwaitinfo */


int prg_aviso(int senal)
{
  struct sigaction accion;
  sigset_t la_demsg;

  accion.sa_flags = SA_SIGINFO;
  accion.sa_sigaction = llega;
  sigemptyset(&accion.sa_mask);
  sigaction(senal, &accion, NULL);   

  /* Enmascaro la senal */

  sigemptyset(&la_demsg); 
  sigaddset(&la_demsg, senal); 
  sigprocmask(SIG_BLOCK, &la_demsg, NULL); 
  return 0;
}

/* Espera mensaje */
/* La senal de notif. sirve para cualquiera de las colas */

int espera_msg(int senal)
{
  sigset_t  mascara;
  siginfo_t info;

  sigemptyset(&mascara);
  sigaddset(&mascara, senal);

  if(sigwaitinfo(&mascara, &info) == -1)
  {
    fprintf(stderr, "segnal inesperada\n");
    fprintf(stderr, "fin %d\n", fin);
    return -1; 
  }
  printf("llega algun mensaje\n");

  /* Hay que reponer la notificacion de la cola correspondiente,
     conocida a traves del dato adicional */

  notif(info.si_value.sival_ptr); 
  return 0;
}

/* Limpieza final: Borra y destruye colas, termina procesos */
/* Los datos necesarios estan en la tabla, y el numero de 
   colas/clientes es n                                      */

int limpia(struct dat_proc *tabla, int n)
{
  int i;
  int estado;
 
  for(i=0; i<n; i++)
  {
    mq_close(tabla[i].cola);
    mq_unlink(tabla[i].ncola);
    fprintf(stderr, "matando cliente %d\n", tabla[i].pid);
    if(kill(tabla[i].pid, SIGUSR1) == (pid_t)-1)
    {
      perror("error en kill");
    }
  }

  /* Me aseguro de que todos terminan */

  for(i=0; i<n; i++)
  {
    printf("esperando clientes\n"); 
    wait(&estado);
  }
}

