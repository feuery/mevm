#ifndef LAMBDA_H
#define LAMBDA_H

#include <opcodes.h>
#include <vector>
#include <stack>
#include <either.h>

using std::vector;
using std::stack;

union value {
  int int_val;
  float float_val;
};

enum primitive { INT, FLOAT };

struct value_container {
  value_container(value vv, primitive t): v(vv), type(t) { }
  value v;
  primitive type;
};
  

struct pointer_container {
  int value;
};

class op {
 public:
  op(opcodes c,
     Either<value_container, pointer_container> *p1 = nullptr,
     Either<value_container, pointer_container> *p2 = nullptr)
  {
    code = c;
    param1_box = p1;
    param2_box = p2;
  }
  
  opcodes code;
  Either<value_container, pointer_container> *param1_box, *param2_box;
};

class Lambda{
public:
  const char* name;
  vector<op> code;
  vector<value_container*> env;
  vector<value_container*> params;
  stack<value_container*> _stack;

  void call();

  value_container* getEnv(pointer_container addr);
  void setEnv(pointer_container addr, value_container* ptr);
  
private:

};

#endif //LAMBDA_H
