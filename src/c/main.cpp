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

  int opt = getopt(argc, argv, "c:");

  if(opt == 'c') {
    printf("Assembling %s\n", optarg);

    generate_code(getLines(optarg), "out.mlisp");
    
    return 0;
  }
  
  Lambda l;
  value k, iso_k;
  k.int_val = 12;
  iso_k.int_val = 3;

  value_container k_c(k, INT),
    iso_k_c(iso_k, INT);
  
  
  l.params.push_back(&k_c);
  l.params.push_back(&iso_k_c);

  auto push_param = create_push_param(200);
  push_param.delete_on_exit = true;

  auto oppi = op(PUSH, &push_param);
  l.code.push_back(oppi);

  auto mul_param = create_mul_param(-2);
  mul_param.delete_on_exit = true;

  auto mul = op(INC, &mul_param);
  l.code.push_back(mul);
  l.call();
  printf("iso_k: %d, k: %d \n", iso_k_c.v.int_val,
	 k_c.v.int_val);
  return 0;
}
