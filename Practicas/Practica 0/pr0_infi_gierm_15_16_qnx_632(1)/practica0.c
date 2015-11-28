/*  Practica 0 INFI-GIERM (10/2015) */

#define _POSIX_C_SOURCE 199506L

/* Cabeceras del sistema */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

/* Cabecera de la practica */

#include "practica0.h"

/* Funcion para proceso de creacion y cierre de graficos */

void crea_grafi(char *nombre, int on);

/* main */

int main(int argc, char **argv) {
   int i;
   pid_t procs[N_VEH];
   int estado;
   pid_t acabado;
   pid_t id_crea_grafi;
            
    /* Crea graficos por medio de proceso hijo crea_grafi */
    /* Si se crearan aqui los graficos no se podrian crear mas procesos */
        
    id_crea_grafi = fork();
    if(id_crea_grafi == 0)  crea_grafi("Practica 0", 1);
    else wait(&i);

    if(WEXITSTATUS(i) == 0) printf("pr0: crea_grafi ha creado los graficos\n");
    else {
       printf("pr0: crea_grafi ha fallado; saliendo\n");
       exit(1);
    }    

    /* Arrancar todos los vehiculos */
        
    i = 0;
    for(i=0; i<N_VEH; i++) {
      char arg1[20];
      char arg2[20];
      char arg3[20];
      char arg4[20];
      
      sprintf(arg1, "%d", i);
      
      procs[i] = fork();
      if(procs[i] == 0) {
         execl("./veh0", "./veh0", arg1, arg2, arg3, arg4, NULL);
         exit(1);
      }
    }    

    /* Esperando fin de vehiculos */

    sleep(5);

    printf("pr0: Todos los procesos de vehiculos han acabado\n");
    
    /* Cerrando graficos mediante crea_grafi */
    
    id_crea_grafi = fork();
    if(id_crea_grafi == 0)  crea_grafi("Practica 0", 0);
    else wait(&i);

    printf("pr0: Acabando\n");
   
    exit(0);
}

/* Funcion para proceso hijo que crea o destruye los graficos */
/* De este modo el proceso padre puede seguir creando procesos */

void crea_grafi(char *nombre, int on)
{
   id_grafi_t idgr;                                                     

   if(on == 1) {
   
      /* Cambia escala y mapa y despues inicializa graficos */
     
     cambia_escala_dib(ESCALA_DIB); 
     cambia_mapa((int *)mapa, ANCHO, ALTO);
    
     printf("crea_grafi: arrancando graficos\n");
     idgr = abrir_graficos(nombre, 1);
     if(idgr != GRAFI_FALLO_CREACION) printf("crea_grafi: Graficos creados\n");
     else {
        printf("crea_grafi: No puede crearse la ventana grafica\n");
        exit(1);
     }     
   } else  {
   
     /* Abre y destruye los graficos */
     
     printf("crea_grafi: Cerrando graficos\n");
     idgr = abrir_graficos(nombre, 0);
     cerrar_graficos(idgr, 1);
   }  
   
   printf("crea_grafi: saliendo\n");
   exit(0);   
}
