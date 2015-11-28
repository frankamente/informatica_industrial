#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#include <stdio.h>

int cnt = 0;
char cadena[20];

void m(int s, siginfo_t *info, void *p)
{
  if(cnt > 18) return;
  cadena[cnt] = info->si_value.sival_int;
  cnt++;
  cadena[cnt] = '\0'; 
}

int main(int argc, char **argv)
{
  sigset_t masc;
  struct sigaction sa;
  int i;
  union sigval valor;
  pid_t peer;

  cadena[0] = '\0';
  sigemptyset(&masc);
  sigaddset(&masc, SIGRTMIN);
  sigprocmask(SIG_BLOCK, &masc, NULL);

  sa.sa_sigaction = m;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGRTMIN, &sa, NULL);
  sigprocmask(SIG_UNBLOCK, &masc, NULL);

  printf("mi pid es %ld\n", (long)getpid());
 
  if(argc >1)
  {
    sscanf(argv[1], "%ld", (long *)&peer);
    printf("Enviando a %ld\n", (long)peer);
    while(!feof(stdin))
    {
       do {
         i = getchar();
         valor.sival_int = i;
         sigqueue(peer, SIGRTMIN, valor);
       } while(i!='\n'&& i!=EOF);
    }
  }
  else
  {
      while(1)
      {
         sleep(1);
         sigprocmask(SIG_BLOCK, &masc, NULL); 
         printf("%s", cadena);
         cnt = 0;
         sigprocmask(SIG_UNBLOCK, &masc, NULL);
      }  
  }
  return 0;
}

#/** PhEDIT attribute block
#-11:16777215
#0:1214:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000116)**/
