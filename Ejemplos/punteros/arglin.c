#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int i;
    int reves = 0;
    int inic = 0;
    int fin =  10;
    
    printf("estos son mis %d argumentos:\n", argc);
    for(i=0; i<argc; i++)
     printf("%s\n", argv[i]);
 
     for(i=1;i<argc;i++)
     {
          if(!strcmp("-r", argv[i]))
          {
               reves = 1;
          }    
          if(!strcmp("-inic", argv[i]))
          {
              if( i+1 < argc)
              {
                if(sscanf(argv[i+1], "%d", &inic)  == 1)
                  i++;
                  else printf("%s no es un numero!\n", argv[i+1]);
               }
          }    

          if(!strcmp("-fin", argv[i]))
          {
               printf("argv %s\n",argv[i+1]);
 
              if( i+1 < argc)
              {
               if(sscanf(argv[i+1], "%d", &fin) == 1)
               i++;
               else printf("%s no es un numero!\n", argv[i+1]);            
               }
          }    
     }

      printf("inic %d fin %d\n", inic, fin);
     if(reves)
     {
             for(i=fin; i>=inic; i--)
           printf("%d\n", i);
     }
     else
     {
         for(i=inic; i<=fin; i++)
           printf("%d\n", i);     
     }
         
     return 0;
}

#/** PhEDIT attribute block
#-11:16777215
#0:709:TextFont14:-3:-3:0
#709:756:TextFont14:0:-1:0
#756:1256:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000171)**/
