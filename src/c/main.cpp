#include <cstdio>
#include <lambda.h>

int main(int argc, char** argv)
{
  Lambda l;
  int k = 1234, iso_k = 300;
  
  l.params.push_back(&k);
  l.params.push_back(&iso_k);

  auto oppi = op(PUSH, 3.5, false);

  l.code.push_back(oppi);
  l.code.push_back(op(MUL, -2, true));
  l.call();
  printf("Tulos: %d\n", iso_k);
  return 0;
}
