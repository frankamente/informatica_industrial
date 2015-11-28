#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 199309

#include <unistd.h>
#include <limits.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  int aux;
 
#ifndef _POSIX_VERSION
  printf("No soporta al POSIX!\n");
#else
  printf("Soporta el POSIX\n");

#if _POSIX_VERSION == 198808
 printf("Soporta el 1 pero no el 4, FIPS 151-1\n");
#else

#if _POSIX_VERSION == 199009
 printf("Soporta el 1 pero no el 4, FIPS 151-2\n");
#else

#if _POSIX_VERSION >= 199309
  printf("Soporta porlo menos el .4; POSIX_VERSION vale %ld\n", 
          (long)_POSIX_VERSION);
#else
  aux = _POSIX_VERSION;
  printf("version %i desconocida o sistema incorrecto\n", aux);
#endif
#endif
#endif
#endif

/* Veamos cuales de las .4 acepta */

#ifdef _POSIX_REALTIME_SIGNALS
  printf("acepta realtime_signals\n");
#endif

#ifdef _POSIX_PRIORITY_SCHEDULING
  printf("acepta prioritized scheduling\n");
#endif

#ifdef _POSIX_TIMERS
  printf("acepta timers\n");
  printf("sysconf: %ld\n", sysconf(_SC_TIMER_MAX));
#ifdef TIMER_MAX
  printf("definido TIMER_MAX\n");
#else
  printf("no esta definido TIMER_MAX\n");
#endif
#else
  printf("NO acepta timers\n");
#endif

#ifdef _POSIX_ASYNCRONOUS_IO
  printf("acepta asyncronous_io\n");
#else
  printf("NO acepta asyncronous_io\n");
#endif

#ifdef _POSIX_PRIORITIZED_IO
  printf("acepta prioritized_io\n");
#else
  printf("NO acepta prioritized_io\n");
#endif

#ifdef _POSIX_SYNCRONIZED_IO
  printf("acepta syncronized_io\n");
#else
  printf("NO acepta syncronized_io\n");
#endif

#ifdef _POSIX_FSYNC
  printf("acepta fsync\n");
#else
  printf("NO acepta fsync\n");
#endif

#ifdef _POSIX_MAPPED_FILES
  printf("acepta mapped files\n");
#else
  printf("NO acepta mapped files\n");
#endif

#ifdef _POSIX_MEMLOCK
  printf("acepta memlock\n");
#endif

#ifdef _POSIX_MEMLOCK_RANGE
  printf("acepta memlock_range\n");
#endif

#ifdef _POSIX_MEMORY_PROTECTION
  printf("acepta memory prot\n");
#endif

#ifdef _POSIX_MESSAGE_PASSING
  printf("acepta mensajes\n");
#endif

#ifdef _POSIX_SEMAPHORES

#ifdef SEM_NSEMS_MAX
  printf("acepta semaforos hasta %d\n", SEM_NSEMS_MAX);
#else
  printf("no esta definido SEM_NSEMS_MAX\n");
#endif

#endif

#ifdef _POSIX_SHARED_MEMORY_OBJECTS
  printf("acepta sh_mem_obj\n");
#endif

  if(sysconf(_SC_SEMAPHORES) != -1) 
    printf("Admite %ld semaforos con valor maximo %ld\n", 
            sysconf(_SC_SEM_NSEMS_MAX), sysconf(_SC_SEM_VALUE_MAX));

  printf("Pueden encolarse hasta %ld senales\n",
          sysconf(_SC_SIGQUEUE_MAX));
  
  printf("Hola mundo\n");
}
