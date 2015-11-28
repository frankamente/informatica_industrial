#include <stdio.h>

int glb;

int suma(int x, int y); 

/* Programa principal */

int main(void)
{
  int i;
  int j;
  int s;
  int s1;

  printf("glb: %p\n", &glb);

  scanf("%d", &i);
  scanf("%d", &j);

  s = suma(i,j);

  printf("Suma: %d\n", s);
}


