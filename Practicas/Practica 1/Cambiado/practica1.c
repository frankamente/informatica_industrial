/* Practica1 (11/2015) */

#define _POSIX_C_SOURCE 199506L

/* Cabeceras del sistema */

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Cabecera de la practica */

#include "practica1.h"

/* Tabla de pids de procesos de vehiculo */

pid_t proc_veh[N_VEH];

/* Prototipos de funciones */

/* Funcion para crear vehiculos */

void crear_vehiculos(void);     

/* Funcion para crear e inicializar graficos */

id_grafi_t  inic_graficos(void);

/* Funcion de inicializacion de senales */

void inic_sigs(void);

/* Funciones de arranque de hilos */

void *almacenes(void *p);          /* Gestion de los almacenes */
void *acceso_pasillo(void *p);    /* Gestion de acceso a pasillos */

/* Manejador */

void mcomun(int sig, siginfo_t *info, void *p)
{
     printf("mcomun: Entra indebidamente manejador en pr1 para senal SIGRTMIN+%d\n", 
        sig-SIGRTMIN);
}
                                     
/* main */

int main(int argc, char **argv) {
   pthread_t h_almacenes;
   pthread_t h_pasillos[N_PASILLOS];
   id_grafi_t idgr;
   sigset_t spuls, sveh;
   siginfo_t info;
   union sigval valor;
   int sig;
   int nveh;
   int npuls;               
   int op;
   int ok;
   int i;               
   
   /* Inicializar tratamiento de senales */
   
   inic_sigs();
   
   sigemptyset(&spuls);
   sigaddset(&spuls, SIG_PULS_CARGA);
   sigaddset(&spuls, SIG_PULS_DESCARGA);

   sigemptyset(&sveh);
   sigaddset(&sveh, SIG_PEDIR_ENCARGO);
         
   /* Crear procesos de vehiculos */

    crear_vehiculos();

    /* Crear graficos */

    idgr = inic_graficos();
    
   /* Crear hilos */

   pthread_create(&h_almacenes, NULL, almacenes, (void *)idgr);
   for(i=0; i<N_PASILLOS; i++)
      pthread_create(&h_pasillos[i], NULL, acceso_pasillo, (void *)i);
   
   /* Bucle de atencion de senales de pulsadores */
   
   while(1) {

       printf("pr1: Esperando alguna senal de pulsador\n");
       sig = sigwaitinfo(&spuls, &info);
       npuls = info.si_value.sival_int;
       if(sig == SIG_PULS_CARGA) {
            printf("pr1: Recibido pulsador de carga %d\n", npuls);
            op = CARGA; 
            ok = 1;
       } else 
       if(sig == SIG_PULS_DESCARGA) {
            printf("pr1: Recibido pulsador de descarga %d\n", npuls);
            op = DESCARGA;
            ok = 1;
       } 
       else {
            printf("pr1: Recibida senal inesperada SIGRTMIN+%d\n", sig-SIGRTMIN);                
            ok = 0;
       }

       /* Esperar peticion de vehiculo */

       if(ok == 1) {
          printf("pr1: Esperando vehiculo disponible\n");    
          sigwaitinfo(&sveh, &info);
          nveh = info.si_value.sival_int;
          printf("pr1: El vehiculo %d se encargara\n", nveh);
          valor.sival_int = op;
          sigqueue(proc_veh[nveh], SIG_RESP_VEH, valor);
          valor.sival_int = npuls;
          sigqueue(proc_veh[nveh], SIG_RESP_VEH, valor);       
       }
   }
     
   printf("pr1: Cerrando graficos\n");  
   cerrar_graficos(idgr, 1);

   printf("pr1: Acabando\n");
   return 0;
}

/* Funcion para hilo que gestiona los almacenes */

void *almacenes(void *p) {
   int i;
   sigset_t sigs;
   id_grafi_t idgr = (id_grafi_t)p;
   int sig;
   siginfo_t info;
   union sigval valor;
   int nveh;
   int posicion;

   /* Tabla de posiciones de almacenamiento */
   
    struct almac {
        int contenido;
        int reserva;
        elemento_t id;
    };         

    struct almac tabla_almac[N_ALM];

   /* Tratamiento de senales */
   
   sigemptyset(&sigs);
   sigaddset(&sigs, SIG_RESERVAR_VACIO);
   sigaddset(&sigs, SIG_RESERVAR_LLENO); 
   sigaddset(&sigs, SIG_DESCARGAR);
   sigaddset(&sigs, SIG_CARGAR);

   /* Inicializar tabla de posiciones de almacenamiento */                
   
   for(i=0; i<N_ALM; i++) {
      tabla_almac[i].id = crear_almacen_id(FILA_ALMACEN, COL_ALMACEN+i, idgr);
      tabla_almac[i].contenido = 0;
      tabla_almac[i].reserva = -1;
   }
             
   /* Bucle de atencion para peticiones */
   
   while(1) {

       printf("almacenes: Esperando alguna senal de peticion\n");
       sig = sigwaitinfo(&sigs, &info);
       nveh = info.si_value.sival_int;
  
       if(sig == SIG_RESERVAR_VACIO) {
          printf("almacenes: El vehiculo %d pide reserva para descargar\n", nveh);              
          for(i=0, posicion = -1; i<N_ALM && posicion == -1; i++) {
          
             if(tabla_almac[i].reserva == -1 && 
                tabla_almac[i].contenido == 0) {   
                 posicion = i;
                 tabla_almac[posicion].reserva = nveh;
             }
          }
          
          if(posicion != -1) printf("almacenes: Reservada posicion %d para %d\n", posicion, nveh);
          else                     printf("almacenes: No hay posiciones vacias\n");
          valor.sival_int = posicion;
          sigqueue(proc_veh[nveh], SIG_RESP_VEH, valor);
       }
       else
       if(sig == SIG_RESERVAR_LLENO) {
          printf("almacenes: El vehiculo %d pide reserva para cargar\n", nveh);              
          for(i=0, posicion = -1; i<N_ALM && posicion == -1; i++) {
            if(tabla_almac[i].reserva == -1 && 
               tabla_almac[i].contenido == 1) {   
                    posicion = i;
                    tabla_almac[posicion].reserva = nveh;
             }
         }
             
          if(posicion !=-1) printf("almacenes: Reservada posicion %d para %d\n", posicion, nveh);
          else                    printf("almacenes: No hay posiciones llenas\n");

          valor.sival_int = posicion;                 
          sigqueue(proc_veh[nveh], SIG_RESP_VEH, valor);
       }
       else
       if(sig == SIG_DESCARGAR) {
           posicion = info.si_value.sival_int;
           printf("almacenes: Peticion de descarga en posicion %d\n", posicion);
           if(tabla_almac[posicion].contenido == 0) {
                printf("almacenes: Descargando de %d\n", posicion);
                tabla_almac[posicion].contenido = 1;
                 tabla_almac[posicion].reserva = -1;   
           }
           else printf("Error: Esta posicion esta ocupada\n");             
       }
       else
       if(sig == SIG_CARGAR) {
            posicion = info.si_value.sival_int;
            printf("almacenes: Peticion de carga de posicion %d\n", posicion);
            if(tabla_almac[posicion].contenido != 0) {
                 printf("almacenes: Cargando de %d\n", posicion);
                 tabla_almac[posicion].reserva = -1;     
                   tabla_almac[posicion].contenido = 0;            
            }
            else printf("Error: No hay nada que cargar; ahi te quedas\n");              

       } 
       else printf("almacenes: Senal desconocida SIGRTMIN+%d\n", sig-SIGRTMIN);

       poner_cont(tabla_almac[posicion].id, tabla_almac[posicion].contenido);
    }       
    return NULL;
}

/* Funcion para hilo de gestion de acceso a un pasillo */

void *acceso_pasillo(void *p)
{
     int id = (int)p;
     siginfo_t info;
     sigset_t spet, slib;
     union sigval val;
     int nveh;
     int sig_pet = SIG_PEDIR_P0 + id;
     int sig_lib = SIG_LIB_P0 + id;
     
     /* Senales */
     
     sigemptyset(&spet); sigaddset(&spet, sig_pet);
     sigemptyset(&slib); sigaddset(&slib, sig_lib);
     
     /* Bucle de atencion de peticiones */
     
     while(1) {
           printf("acceso_pasillo-%d: Esperando senal de peticion\n", id);
           
           sigwaitinfo(&spet, &info);
        
          
          
          
          
          
          
           nveh = info.si_value.sival_int;
           printf("aceso_pasillo-%d: Recibida peticion de vehiculo %d\n", id, nveh);
           
          sigqueue(proc_veh[nveh], SIG_RESP_VEH, val);
          
           
           printf("aceso_pasillo-%d: Concedido pasillo %d a vehiculo %d\n", id, id, nveh);          
  sigwaitinfo(&slib, &info);
           printf("aceso_pasillo-%d: Pasillo %d liberado\n", id, id);
     }
}

/* Funcion para inicializar senales */

void inic_sigs(void) {
   struct sigaction accion;
   sigset_t sigs;
   int i;
         
   for(i=SIG_PULS_CARGA; i<=SIG_LIB_P3; i++)
       sigaddset(&sigs, i);
	   pthread_sigmask(SIG_BLOCK, &sigs, NULL);
   accion.sa_flags = SA_SIGINFO;
   sigemptyset(&accion.sa_mask);
   accion.sa_sigaction = mcomun;

   for(i=SIG_PULS_CARGA; i<=SIG_LIB_P3; i++)
      sigaction(i, &accion, NULL);
}

/* Funcion para cear procesos de vehiculos */

void crear_vehiculos(void) {  
    int i;
  for(i=0; i<N_VEH; i++) {
      char arg1[10];
      char arg2[10];
      char arg3[10];
      sprintf(arg1, "%d", i);
      sprintf(arg2, "%d", FILA_APARCAMIENTO);
      sprintf(arg3, "%d", COL_APARCAMIENTO + i);
      proc_veh[i] = fork();
      if(proc_veh[i] == 0) {
          execl("./veh1", "./veh1", arg1, arg2, arg3, NULL); 
          printf("pr1: No puedo arrancar vehiculo %d\n", i);
          exit(1);
       }
       else printf("pr1: Creado proceso de vehiculo %d\n", i);
    }
}

/* Crear graficos */
/* NO ES NECESARIO MODIFICARLA */

id_grafi_t inic_graficos(void) {

   id_grafi_t  idgr;
   int i;
   char nombre[20];
         
   /* Crear y abrir graficos */

   printf("pr1: arrancando graficos\n");
   idgr = abrir_graficos("Practica 1", 1);
   if(idgr != GRAFI_FALLO_CREACION) printf("crea_grafi: Graficos creados\n");
   else {
      printf("pr1: No puede crearse la ventana grafica\n");
      exit(1);
   }
   
   /* Cambiar escala y fondo */
        
   cambia_escala_id(ESCALA_