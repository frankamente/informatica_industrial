/* Ejemplo de uso de la interfaz grafica */

/* Cabeceras */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <math.h>
#include "api_grafi.h"         /* Interfaz grafica */

#define PI 3.14159265358979

/* Dimensiones y escala del grafico */

#define ESCALA_DIB 30


#define  NPUNTOS      32
#define  D_SEGURIDAD 2.

/* main */

int main(void)
{      
   id_grafi_t pgrafi;
   elemento_t veh;
   int i;
   float barrido[NPUNTOS];
   float angulos[NPUNTOS];
   struct timespec ciclo = {0, 200000000L};
   float min;  /* Distancia  minima */
   int imin;  /* Indices de distancias min y max */
   float vf; float vc; float kv; float val;
       
   /* Arranque de graficos */
   
   pgrafi = abrir_graficos("migrafi", 0);
   if(pgrafi == (id_grafi_t)NULL) exit(1);
   else printf("main: Tengo acceso a ventana migrafi\n");

   /* Semilla para la secuencia pseudo-aleatoria */     

   srand((int)getpid());

  /* Crear vehiculo */
        
  veh = crear_vehiculo_id(2,2, pgrafi);
  configurar_laser(veh, NPUNTOS, -180., +180., 0., 100.);
  
  for(i=0; i<NPUNTOS; i++)
    angulos[i] = -PI + i*2*PI/NPUNTOS;
 
   kv = 2.;
   val = 4.;
   
   while(1)
   {   
      leer_laser(veh, barrido, NPUNTOS);
  
      /* Minimo */
      
     min = 1e6;
     for(i=0; i<NPUNTOS; i++)
     {
	    if(min > barrido[i])
	    {
	       min = barrido[i];
	       imin = i;
	    } 
     }
       
      if(min < D_SEGURIDAD)
      {
          vf =   kv*sin(angulos[imin]); 
          vc = -kv*cos(angulos[imin]);
      }
      else
      {
         vf=rand()*2*val/RAND_MAX -val;
         vc=rand()*2*val/RAND_MAX -val;
      }
 
      cambia_vel(veh, vf, vc);
      nanosleep(&ciclo, NULL);         
   }

   /* Aqui no llega.... */
   
   printf("main: Parando graficos\n");
   cerrar_graficos(pgrafi, 0);
   printf("main: Acabando\n");
   exit(0);
}



