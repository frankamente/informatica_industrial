#include <time.h>

int main(void)
{   
   time_t inicio;
   time_t ahora;

   inicio = time(NULL);
   printf("inicio: %ld\n", inicio);
   while( ( (time(NULL)) - inicio) < 10);
}
