#include <stdio.h>
#include <stdlib.h>

float **fmat(int ancho, int alto);
void libera_fmat(float **m, int alto);

int main(int argc, char **argv)
{
  float **m;
  int ancho, alto;
  int i,j;

  scanf("%d %d", &ancho, &alto);
  printf("Has tecleado %d %d\n", ancho, alto);

  m = fmat(ancho, alto);

  for(i=0; i<alto; i++)
    for(j=0; j<ancho; j++)
       m[i][j] = i+j;

  for(i=0; i<alto; i++)
  {
    for(j=0; j<ancho; j++)
     printf("%f ", m[i][j]);
    printf("\n");
  }

  libera_fmat(m, alto);

}

/* Reserva almacenamiento contiguo */
float **fmat(int ancho, int alto)
{
  int i;
  float **res;
  float *pun;
  
  res = (float **)malloc(sizeof(float *)*alto);
  pun = (float *)malloc(ancho*alto*sizeof(float));
  if(res == NULL || pun == NULL)
  {
     printf("No hay memoria\n");
     return NULL;
   }

   for(i=0; i<alto; i++, pun+=ancho)
     res[i] = pun;  

  /*
  for(i=0; i<alto; i++)
    res[i] = malloc(sizeof(float)*ancho);
  */
  return res;
}

void libera_fmat(float **m, int alto)
{
  int i;
/*
  for(i=0; i<alto; i++)
     free(m[i]); */

   free(m[0]);
      
  free(m);
}

#/** PhEDIT attribute block
#-11:16777215
#0:1101:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000116)**/
