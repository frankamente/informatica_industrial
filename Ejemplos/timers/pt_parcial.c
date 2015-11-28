#include <time.h>

int main(void)
{
   struct timespec tim = {0, 500000000L};
   struct timespec antes;
   struct timespec ahora;
   double t;

   while(1)
   { 
      nanosleep(&tim, NULL);
      clock_gettime(CLOCK_REALTIME, &ahora);
      antes = ahora;
      do
      {
        clock_gettime(CLOCK_REALTIME, &ahora);
        t = (ahora.tv_sec - antes.tv_sec) + ((double)(ahora.tv_nsec-antes.tv_nsec))*1e-9;
      }
      while(t < 0.600);
   }

}
