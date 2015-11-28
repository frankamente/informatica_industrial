/* modulo1.c */

#include <stdio.h>

void f1(void);
int fext(int y);
int privada(int x);

/* Programa principal */

extern int externa1;
extern int externa3;

int main(void)
{
  int i;
  i = externa2;

  printf("externa1: %d\n", externa1);
  printf("externa3: %d\n", externa3);

  f1();
  f1();

  printf("fext: %d\n", fext(4));

  privada(i);

}

void f1(void)
{
  int aux;
  int externa3;
  static int estatica;
  
  externa3 = 7;
  aux = i;

  printf("externa1: %d\n", externa1);
  printf("externa3: %d\n", externa3);
  printf("estatica: %d\n", estatica);
 
  estatica = estatica + 1;
}
