#define _POSIX_C_SOURCE 199309L

#include <time.h>

int main(int argc, char **argv)
{
   time_t tiempo;
   struct tm *fecha;

   tiempo = time(NULL);

   printf("tiempo %ld\n", tiempo);

   fecha = gmtime(&tiempo);   

   printf("agno %d mes %d dia %d hora %d minuto %d segundo %d\n",
           fecha->tm_year, fecha->tm_mon, fecha->tm_mday, 
           fecha->tm_hour, fecha->tm_min, fecha->tm_sec);
    printf("tiempo en char %s\n", asctime(fecha));

   fecha = localtime(&tiempo);   

   printf("agno %d mes %d dia %d hora %d minuto %d segundo %d\n",
           fecha->tm_year, fecha->tm_mon, fecha->tm_mday, 
           fecha->tm_hour, fecha->tm_min, fecha->tm_sec);
    printf("tiempo en char %s\n", asctime(fecha));
}


#/** PhEDIT attribute block
#-11:16777215
#0:726:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
