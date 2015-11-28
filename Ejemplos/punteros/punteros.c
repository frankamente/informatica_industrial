#include <stdio.h>

int glb = 8;

int main(int argc, char **argv)
{
  int aut1;
  int aut2;
  double f;
  char *p = "cad1";
  char v[] = "cad2";
  int *p1;
  int  *p2;
  char *pc1;
  char *pc2;
  
  p1 = &aut1;
  p2 = &aut2;
  pc1 = v+1;
  pc2 = v + 3; /* &v[3] */  
  printf("&glb: %p &aut1: %p, %p &aut2, &f: %p\n", 
        &glb, &aut1, &aut2, &f);

  printf("&p: %p, p: %p, v: %p\n",
            &p, p, v);
            
  printf("p1 %p p2 %p pc1 %p pc2 %p\n", p1, p2, pc1, pc2);
  
  printf("dif p2-p1: %d   dif pc2-pc1 %d\n", p2-p1, pc2-pc1);
  
  printf("dif convirtiendo a unsigned int: p2-p1 %d  pc2-pc1 %d\n", 
  (unsigned int)p2 - (unsigned int)p1, (unsigned int)pc2 - (unsigned int)pc1);
  
  return 0;
}

#/** PhEDIT attribute block
#-11:16777215
#0:716:TextFont14:-3:-3:0
#**  PhEDIT attribute block ends (-0000115)**/
