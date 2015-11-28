#include <stdio.h>

int main(int argc, char **argv)
{
  int i;

  printf("argc vale %d\n", argc);

  printf("Estos son mis argumentos:\n");

  for(i=0; i<argc; i++)
    printf("%s\n", argv[i]);
    
}

#/** PhEDIT attribute block
#-11:16777215
#0:198:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
