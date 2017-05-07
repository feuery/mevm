#include <cassert>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <lambda.h>
#include <assembler.h>

Either<value_container, pointer_container> create_push_param(int vv)
{
  Either<value_container, pointer_container> toret;
  value v;
  v.int_val = vv;
  toret.a = new value_container(v, INT);

  return toret;
}

Either<value_container, pointer_container> create_mul_param(int addr)
{
  Either<value_container, pointer_container> toret;
  toret.b = new pointer_container;
  toret.b->value = addr;

  return toret;
}

int main(int argc, char** argv)
{
  // man 3 getopt

  assert(sizeof(int*) == 4);
  assert(sizeof(long long) == 8);
  
  int opt = getopt(argc, argv, "c:");

  if(opt == 'c') {
    printf("Assembling %s\n", optarg);

    generate_code(getLines(optarg), "out.mlisp");
    
    return 0;
  }

  puts("Currently supported opts: -c ./path/to/assembly.mlispa");
  
  return 0;
}
