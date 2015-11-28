/* Cabecera para practica 1 (11/2014) */
/* Copyright (C) Joaquin Ferruz Melero 2014 */

#ifndef __PRACTICA1_H__
#define __PRACTICA1_H__

/* Acceso a interfaz grafica */

#include "api_grafi.h"

/* Senales de pulsadores recibidas por pr1 */

#define SIG_PULS_CARGA         	SIGRTMIN  			/* Senal generada por los pulsadores de carga */
#define SIG_PULS_DESCARGA   	SIGRTMIN+1			/* Senal generada por los pulsadores de descarga */

/* Senales procedentes de vehiculos y recibidas por pr1 */

#define SIG_PEDIR_ENCARGO    	SIGRTMIN+2       	/* Pedir encargo de operacion de carga o descarga */
#define SIG_RESERVAR_VACIO    	SIGRTMIN+3        	/* Reservar posicion de almacen vacia */
#define SIG_RESERVAR_LLENO   	SIGRTMIN+4       	/* Reservar posicion de almacen llena */
#define SIG_DESCARGAR            	SIGRTMIN+5 			/* Descargar en posicion de almacen */
#define SIG_CARGAR                  	SIGRTMIN+6			/* Cargar en posicion de almacen */
#define SIG_PEDIR_P0                	SIGRTMIN+7			/* Pedir pasillo 0 */
#define SIG_PEDIR_P1                	SIGRTMIN+8			/* Pedir pasillo 1 */
#define SIG_PEDIR_P2                	SIGRTMIN+9			/* Pedir pasillo 2 */
#define SIG_PEDIR_P3                	SIGRTMIN+10		/* Pedir pasillo 3 */
#define SIG_LIB_P0                     	SIGRTMIN+11		/* Liberar pasillo 0 */
#define SIG_LIB_P1                    	SIGRTMIN+12		/* Liberar pasillo 1 */
#define SIG_LIB_P2                    	SIGRTMIN+13		/* Liberar pasillo 2 */
#define SIG_LIB_P3                    	SIGRTMIN+14		/* Liberar pasillo 3 */

/* Senal de respuesta enviada por pr1 y recibida por el vehiculo */

#define SIG_RESP_VEH              	SIGRTMIN				/* Respuesta comun para cualquier peticion */

/* Tipo de operacion para enviar con SIG_RESP_VEH */

enum tipo_op { CARGA = 			0,    	/* Cargar en zona de pulsadores de carga */
                         DESCARGA = 	1    	/* Descargar en zona de pulsadores de descarga */
                      };

/* Dimensiones del dibujo */

#define ANCHO                    	11
#define ALTO                       	11
#define ESCALA_DIB            		40

/* Numero de elementos */

#define N_PULS			        	3
#define N_VEH                      	4
#define N_ALM                      	3
#define N_PASILLOS              	4

/* Filas y columnas */

#define FILA_PULS_CARGA      	0
#define COL_PULS_CARGA      	7
#define FILA_APARCAMIENTO   	1
#define COL_APARCAMIENTO   	1
#define FILA_ALMACEN          	4
#define COL_ALMACEN            	7
#define FILA_PULS_DESCARGA	7
#define COL_PULS_DESCARGA	7

#define COL_IDA                    	5      
#define FILA_VUELTA    			ALTO-2     
#define COL_VUELTA            	1   

/* Mapa:
    C: camino
    P: Pared
 */

#define P PARED                    /* Definido en api_grafi.h */
#define C PASILLO                  /*   "              "           "     */

int mapa[ALTO][ANCHO] = 
                  {
                    {P, P, P, P, P, P, P, P, P, P, P},
                    {P, C, C, C, C, P, P, C, C, C, P},
                    {P, C, C, C, C, C, C, C, C, C, P},
                    {P, C, P, P, P, C, P, P, P, P, P},
                    {P, C, P, P, P, C, P, C, C, C, P},
                    {P, C, P, P, P, C, P, C, C, C, P},
                    {P, C, P, P, P, C, C, C, C, C, P},
                    {P, C, P, P, P, C, P, P, P, P, P},
                    {P, C, P, P, P, C, P, C, C, C, P},
                    {P, C, C, C, C, C, C, C, C, C, P},
                    {P, P, P, P, P, P, P, P, P, P, P}
                  };

#endif

#/** PhEDIT attribute block
#-11:16777215
#0:2653:default:-3:-3:0
#2653:2689:TextFont11:0:-1:0
#2689:3514:default:-3:-3:0
#**  PhEDIT attribute block ends (-0000169)**/
