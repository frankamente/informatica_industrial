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
   char *p;
   
 

 int n_veh, por_defecto; // por_defecto=1 si no tengo argumentos o no tengo suficientes
 
 char **paso;

 ////////////////////////////////////////////////////////////////////// ARGUMENTOS*/   
            /* Si tengo alg�n argumento, es el n�mero de vehiculos y hago lo dem�s */
            
              printf("ARGUMENTOS: %d\n",argc-1);
 if (argc >0)
 {
   n_veh  = atoi(argv[1]);
    printf("N_VEH: %d\n",n_veh);
 }

  if (argc <(4*n_veh+1+1))
   {
  		printf(" Me faltan argumentos \n");
		por_defecto=1;	
		n_veh=N_VEH;
    }
    
    else
    {
    	printf(" Numero de argumentos correctos \n");
     	por_defecto=0;
     }     
            
 
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////        
  
            
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
    for(i=0; i<n_veh; i++) {
      char arg1[20];
      char arg2[20];
      char arg3[20];
      char arg4[20];
      
      if(!por_defecto)
      {
      sprintf(arg1, "%d", i); // Nombre del vehiculo
      sprintf(arg2, "%d", i+1); //Fila de inicio ///////////////////////// CAMBIADO
      sprintf(arg3, "%d", i+1); // Columna de inicio ///////////////////////// CAMBIADO
      sprintf(arg4, "%d", 3); // Numero de choques  ///////////////////////// CAMBIADO
      }
      else
      {
      int argu1=(int)argv[(2+((n_veh-1)*4))];
    	 int argu2=argv[3+(n_veh-1)*4];
      arg3=argv[4+(n_veh-1)*4];
      arg4=argv[5+(n_veh-1)*4];*/
      }
      
      procs[i] = fork();
      if(procs[i] == 0) {
         execl("./veh0", "./veh0", arg1, arg2, arg3, arg4, NULL);  ///////////////////////// CAMBIADO
         exit(1);
      }
    }    

    /* Esperando fin de vehiculos */

    //sleep(10);  ///////////////////////// CAMBIADO
  for(i=0; i<N_VEH; i++) { /////////////////////// CAMBIADO
 acabado=wait(&estado); ///////////////////////// CAMBIADO
  
}
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
