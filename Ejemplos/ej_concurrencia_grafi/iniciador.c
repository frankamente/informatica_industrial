/* Inicializador de la interfaz grafica */

/* Cabeceras */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include "api_grafi.h"         /* Interfaz grafica */

/* Dimensiones y escala del grafico */

#define ESCALA_DIB 30
#define ALTO 10
#define ANCHO 10

int fondo[ALTO][ANCHO] = {
                             { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                             { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
}; 

int main(void)
{      
   id_grafi_t pgrafi;
       
   /* Arranque de graficos */
   
   pgrafi = abrir_graficos("migrafi", 0);
   cambia_escala_id(ESCALA_DIB, pgrafi);
   crear_fondo_id(fondo[0], ANCHO, ALTO, pgrafi);

   printf("Ya he inicializado esto\n");
   cerrar_graficos(pgrafi, 0);
   printf("main: Acabando\n");
   exit(0);
}



