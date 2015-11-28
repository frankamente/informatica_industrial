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

  printf("dirs: glb %p, i %p, j %p, s %p\n",
          &glb, &i, &j, &s);

  scanf("%d", &i);
  scanf("%d", &j);

  s = suma(i,j);

  printf("Suma: %d\n", s);
}

/* Rutina de suma */

int suma(int x, int y)
{
  int aux;

  printf("dirs: x %p y %p aux %p\n", &x, &y, &aux); 
  aux = x+y;

  return aux;
}

