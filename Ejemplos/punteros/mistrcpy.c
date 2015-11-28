#include <stdio.h>
#include <string.h>

char *micpy(char *orig, char *dest);
char *micpy1(char *orig, char *dest);
char *micpy2(char *orig, char *dest);

int main(int argc, char **argv)
{
  char cad1[] = "cadena uno\n";
  char dest[20] ; 
  int i;
  
  for(i=0; i<20; i++) dest[i] = 'p';
  
  printf("dest %p, cad1 %p\n", dest, cad1);
  
  micpy(dest, cad1);
  
  printf("destino:%s\n", dest); 

  micpy1(dest, cad1);
  
  printf("destino:%s\n", dest); 
  
}

char *micpy(char *dest, char *orig)
{
     int i;
     
     for(i=0; orig[i] != '\0'; i++)
         dest[i] = orig[i];
         
     /*dest[i] = orig[i]; */
     
     return dest;
}

char *micpy1(char *dest, char *orig)
{
    char *salvar = orig;
    
    while((*dest++ = *orig++) != '\0');
    
    return salvar;
}
