#include <cstdio>
#include <lambda.h>

int main(int argc, char** argv)
{
  Lambda l;
  float k = 12.34, iso_k = 3.7;
  
  l.params.push_back((int*)&k);
  l.params.push_back((int*)&iso_k);

  auto oppi = op(PUSH, k, false, true);

  l.code.push_back(oppi);
  l.code.push_back(op(MUL, -2, true, true));
  l.call();
  printf("Tulos: %f\n", iso_k);
  return 0;
}
