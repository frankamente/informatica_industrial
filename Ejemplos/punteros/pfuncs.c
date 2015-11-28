
#include <stdio.h>
#include <math.h>

double cuad(double);
double cubo(double);

double (*tabla[])(double) = {sin, cos, tan, cuad, cubo};

int main(int argc, char **argv)
{
  float valor;
  int n;
  int max;

  if(argc < 3) 
  { 
    printf("no hay suf. args.\n");
    exit(1);
  }

  sscanf(argv[1], "%d", &n);
  sscanf(argv[2], "%f", &valor);

  max = sizeof(tabla)/sizeof(double (*)(double));
  if(n<0 || n>= max) 
  {
    printf("no existe la funcion %d\n", n);
    exit(1);
  }

  printf("resultado %f\n", tabla[n](valor));

  return 0;
}

double cuad(double v)
{
  return v*v;
}

double cubo(double v)
{
  return v*v*v;
}
