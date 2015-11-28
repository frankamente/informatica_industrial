#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
  time_t tiempo;
  time_t tiempo1;

  tiempo1 = tiempo = time(NULL);
  printf("t %ld\n", (long)tiempo);

  while(tiempo1-tiempo < 10)
  {
    sleep(1);
    tiempo1 = time(NULL);
    printf("t %ld\n", (long)tiempo1);
  }
}
