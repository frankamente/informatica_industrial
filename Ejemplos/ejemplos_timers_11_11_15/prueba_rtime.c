#define _POSIX_C_SOURCE 199309L

#include <time.h>


int main(int argc, char **argv)
{
   time_t tiempo;
   struct timespec anterior;
   struct timespec rtiempo;
   int i;
   float inc;

   for(i=0; i<10; i++)
   {
     sleep(3);
     tiempo = time(NULL);
     clock_gettime(CLOCK_REALTIME, &rtiempo);
     if(!i) anterior = rtiempo;
     inc = rtiempo.tv_sec - anterior.tv_sec + 
           (rtiempo.tv_nsec-anterior.tv_nsec)*1e-9;
     printf("tiempo %ld rtiempo %ld sg %ld ns inc %f\n", 
               (long)tiempo, 
               (long)rtiempo.tv_sec, (long)rtiempo.tv_nsec,
                 inc);
     anterior = rtiempo;
   }
}
