
int externa1 = 27;
static int externa3 = 18;
int externa2;

int fext(int y)
{
  externa2 = y;
  return externa3;

}

static void privada(int x)
{
  externa3 = x;
}
