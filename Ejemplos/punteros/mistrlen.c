#include <stdio.h>
#include <string.h>

size_t mistrlen(char *cad);
size_t mistrlen1(char *cad);

int main(int argc, char **argv)
{
  char cad1[] = "cadena uno\n";

  printf("cadena1 ocupa %d octetos\n", sizeof(cad1));

  printf("longitud cad1: %d; segun strlen: %d segun mistrlen1: %d\n", mistrlen(cad1), strlen(cad1), mistrlen1(cad1));

  
}

size_t mistrlen(char *cad)
{
  int i;

  for(i=0; cad[i] != '\0'; i++);
  return i;
    
}

size_t mistrlen1(char *cad)
{
     char *pcom = cad;
     
     while(*cad != '\0') cad++;
     
     return cad-pcom;
}