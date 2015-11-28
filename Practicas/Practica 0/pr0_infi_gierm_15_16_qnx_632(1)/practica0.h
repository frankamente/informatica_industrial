/* Cabecera para practica 0 GIERM (10/2015) */
/* Copyright (C) Joaquin Ferruz Melero 2015 */

#ifndef __PRACTICA0_H__
#define __PRACTICA0_H__

/* Acceso a interfaz grafica */

#include "api_grafi.h"

/* Dimensiones del dibujo */

#define ANCHO 9               /* Ancho del mapa */
#define ALTO 9                  /* Alto del mapa */
#define ESCALA_DIB 40      /* Escala */
#define N_VEH 4                /* Numero de vehiculos */

/* Mapa:
    C: camino
    P: Pared  */

#define P PARED                    /* Definido en api_grafi.h */
#define C PASILLO                  /*   "              "           "     */


int mapa[ALTO][ANCHO] = {
                        {P, P, P, P, P, P, P, P, P},
                        {P, C, C, C, C, C, C, C, P}, 
                        {P, C, C, C, C, P, P, C, P}, 
                        {P, C, P, C, C, C, P, C, P}, 
                        {P, C, C, C, C, C, C, C, P}, 
                        {P, C, P, P, C, C, C, P, P}, 
                        {P, C, C, P, C, C, C, C, P}, 
                        {P, C, C, C, C, C, C, C, P}, 
                        {P, P, P, P, P, P, P, P, P}
                      };
#endif

