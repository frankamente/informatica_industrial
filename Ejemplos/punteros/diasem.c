#include <stdio.h>


int main(void)
{
  int n;
  char *dias[] = { "lunes", "martes", "miercoles", 
                  "jueves", "viernes", "sabado", "domingo" };

  printf("Que dia? ");
  scanf("%d", &n);

  if(n<1 || n>7) printf("No hay tal dia\n");
  else           printf("El dia %d es %s\n", n, dias[n-1]);
}

